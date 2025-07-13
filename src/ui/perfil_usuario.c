#include <ui/interfaces.h>
#include <time.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

/* ======== PCRE2: Validadores internos ======== */

static pcre2_code *get_email_re()
{
    static pcre2_code *re = NULL;
    if (re)
        return re;

    int errornumber;
    PCRE2_SIZE erroroffset;
    PCRE2_SPTR pattern = (PCRE2_SPTR) "^([a-zA-Z0-9]+([._-][a-zA-Z0-9]+)*)@([a-zA-Z0-9]+(-[a-zA-Z0-9]+)*)(\\.[a-zA-Z]{2,})+$";

    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (!re)
    {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        fprintf(stderr, "Error al compilar regex email: %s\n", buffer);
    }
    return re;
}

static pcre2_code *get_password_re()
{
    static pcre2_code *re = NULL;
    if (re)
        return re;

    int errornumber;
    PCRE2_SIZE erroroffset;
    PCRE2_SPTR pattern = (PCRE2_SPTR) "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[^A-Za-z0-9]).{8,}$";

    re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (!re)
    {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        fprintf(stderr, "Error al compilar regex password: %s\n", buffer);
    }
    return re;
}

static bool validar_email(const char *email)
{
    if (!email)
        return false;
    pcre2_match_data *md = pcre2_match_data_create_from_pattern(get_email_re(), NULL);
    int rc = pcre2_match(get_email_re(), email, strlen(email), 0, 0, md, NULL);
    pcre2_match_data_free(md);
    return rc >= 0;
}

static bool validar_password(const char *password)
{
    if (!password)
        return false;
    pcre2_match_data *md = pcre2_match_data_create_from_pattern(get_password_re(), NULL);
    int rc = pcre2_match(get_password_re(), password, strlen(password), 0, 0, md, NULL);
    pcre2_match_data_free(md);
    return rc >= 0;
}

/* ========== Validaciones internas ========== */

typedef struct
{
    bool *check;
    const char *email;
} search_email_arg_t;

static new_operfn(check_email_repetido)
{
    search_email_arg_t *argu = arg;
    Usuario *u = val;
    if (*argu->check)
        return FOREACH_BREAK;
    if (strcmp(u->email, argu->email))
        return FOREACH_CONTINUE;
    *argu->check = true;
    return FOREACH_BREAK;
}

/* ========== Mensajes generales ========== */

/* ========== Funciones de interfaz ========== */

static interfaz(CrearCuenta)
{
    const char *email = argv[0];
    const char *username = argv[1];
    const char *password = argv[2];
    const char *confirmPassword = argv[3];
    const char *nickname = argv[4];
    const char *pais = argv[5];

    if (!email || !username || !password || !confirmPassword || !nickname || !pais)
    {
        send_message("Todos los campos son obligatorios");
        return false;
    }

    if (!validar_email(email))
    {
        send_message("Correo no válido");
        return false;
    }

    bool emailRepetido = false;
    search_email_arg_t wrapper_arg = {&emailRepetido, email};
    forEachInABB_InOrder(usuarios, check_email_repetido, &wrapper_arg);
    if (emailRepetido)
    {
        send_message("El email ya está registrado");
        return false;
    }

    if (searchValueInABB(usuarios, username, cmpUsuarioConUsername))
    {
        send_message("El username ya existe");
        return false;
    }

    if (!validar_password(password))
    {
        send_message("La contraseña no es válida");
        return false;
    }

    if (strcmp(password, confirmPassword))
    {
        send_message("Las contraseñas no coinciden");
        return false;
    }

    char *datos = asprintf(stringify("%s", "%s", "%s", "%s", "%s"), username, email, password, nickname, pais);
    nuevo_registro("Usuarios", "username, email, password, nickname, pais", datos, NULL);

    char *cond = asprintf(stringify(username = "%s"), username);
    obtener_registros("Usuarios", "*", cond, cargarUsuarios, NULL, NULL);
    freem(cond);

    // Reportar éxito
    send_message("Cuenta creada exitosamente");

    return true;
}

json_object *usuario_to_json(Usuario *u)
{
    if (!u)
        return NULL;

    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "name", json_object_new_string(u->nickname));
    json_object_object_add(jobj, "username", json_object_new_string(u->username));
    json_object_object_add(jobj, "email", json_object_new_string(u->email));
    json_object_object_add(jobj, "country", json_object_new_string(u->pais));
    json_object_object_add(jobj, "isArtist", json_object_new_boolean(u->artista != NULL));

    // Subobjeto de suscripción
    json_object *subscription = json_object_new_object();

    const char *tipo_plan = (u->plan == PLAN_PREMIUM) ? "premium" : "freemium";
    json_object_object_add(subscription, "type", json_object_new_string(tipo_plan));
    json_object_object_add(subscription, "autoRenewal", json_object_new_boolean(true)); // Asumido

    // Fecha de expiración: 180 días desde hoy
    time_t now = time(NULL);
    now += 180 * 24 * 60 * 60;
    struct tm *exp_date = localtime(&now);
    char fecha_exp[11];
    strftime(fecha_exp, sizeof(fecha_exp), "%Y-%m-%d", exp_date);
    json_object_object_add(subscription, "expiration", json_object_new_string(fecha_exp));

    json_object_object_add(jobj, "subscription", subscription);

    return jobj;
}

/* ================ Funciones para actualizar datos del usuario ================ */

interfaz(ActualizarNickname)
{
    const char *nickname = argv[0];

    // Validar que el campo no sea nulo
    if (!nickname || !*nickname)
    {
        send_message("El nickname no puede estar vacío");
        return false;
    }

    // Liberar la memoria del antiguo valor
    freem(usuario->nickname);

    // Actualizar al nuevo valor
    usuario->nickname = asprintf(nickname);

    // Reportar éxito
    send_message("Nickname actualizado exitosamente");

    return true;
}

interfaz(ActualizarEmail)
{
    const char *email = argv[0];

    // Validar que el email no sea nulo
    if (!email || !*email)
    {
        send_message("El email no puede estar vacío");
        return false;
    }

    // Validar que el valor haya cambiado
    if (!strcmp(usuario->email, email))
    {
        send_message("El email no ha cambiado.");
        return false;
    }

    // Validar formato de email
    if (!validar_email(email))
    {
        send_message("Correo no válido");
        return false;
    }

    // Verificar que el email no esté registrado en otro usuario

    bool emailRepetido = false;

    search_email_arg_t wrapper_arg =
        {
            .check = &emailRepetido,
            .email = email,
        };

    forEachInABB_InOrder(usuarios, check_email_repetido, &wrapper_arg);

    // Si el email ya está registrado, se envia un mensaje
    if (emailRepetido)
    {
        send_message("El email ya está registrado.");
        return false;
    }

    // Liberar la memoria del antiguo valor
    freem(usuario->email);

    // Actualizar al nuevo valor
    usuario->email = asprintf(email);

    // Reportar éxito
    send_message("Email actualizado exitosamente");

    return true;
}

interfaz(ActualizarPais)
{
    const char *pais = argv[0];

    // Validar que el campo no sea nulo
    if (!pais || !*pais)
    {
        send_message("El país no puede estar vacío");
        return false;
    }

    // Liberar la memoria del antiguo valor
    freem(usuario->pais);

    // Actualizar al nuevo valor
    usuario->pais = asprintf(pais);

    // Reportar éxito
    send_message("Pais actualizado exitosamente");

    return true;
}

interfaz(ActualizarPassword)
{
    const char *currentPassword = argv[0];
    const char *newPassword = argv[1];
    const char *confirmPassword = argv[2];

    // Validar que no sean campos nulos o vacios
    if (!currentPassword || !*currentPassword ||
        !newPassword || !*newPassword ||
        !confirmPassword || !*confirmPassword)
    {
        send_message("Los campos no pueden estar vacios.");
        return false;
    }

    // Validar que el campo de la contraseña actual coincida
    if (strcmp(usuario->password, currentPassword))
    {
        send_message("La contraseña actual es incorrecta.");
        return false;
    }

    // Validar que el valor haya cambiado
    if (!strcmp(currentPassword, newPassword))
    {
        send_message("La nueva contraseña debe ser diferente.");
        return false;
    }

    // Validar el patrón de la nueva contraseña
    if (!validar_password(newPassword))
    {
        send_message("La contraseña no es valida");
        return false;
    }

    // Validar que la nueva contraseña y su confirmación coincidan
    if (strcmp(newPassword, confirmPassword))
    {
        send_message("Las contraseñas no coinciden.");
        return false;
    }

    // Liberar la memoria del antiguo valor
    freem(usuario->password);

    // Actualizar al nuevo valor
    usuario->password = asprintf(newPassword);

    // Reportar éxito
    send_message("Contraseña actualizada exitosamente.");

    return true;
}

message_handler(crear_cuenta)
{
    init_data_json();
    const char *nickname = get_string(get_array_idx(data, 0));
    const char *pais = get_string(get_array_idx(data, 1));
    const char *username = get_string(get_array_idx(data, 2));
    const char *email = get_string(get_array_idx(data, 3));
    const char *password = get_string(get_array_idx(data, 4));
    const char *confirmPassword = get_string(get_array_idx(data, 5));

    const char *datos[] = {email, username, password, confirmPassword, nickname, pais};

    char **msg = arg;
    bool ok = VibeCast_CrearCuenta(NULL, 6, datos, msg);
    VibeCast_SendBool(id, ok ? HTTP_CREATED : HTTP_BAD_REQUEST, ok, *msg, ok ? STATE_SUCCESS : STATE_FAILURE);
    freem(*msg);
    *msg = NULL;
}

message_handler(eliminar_cuenta)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(obtener_info_usuario)
{
    if (!usuario)
    {
        VibeCast_SendBool(id, HTTP_UNAUTHORIZED, false, "No hay sesión activa", STATE_FAILURE);
        return;
    }

    json_object *jobj = usuario_to_json(usuario);

    VibeCast_SendJSON(id, HTTP_OK, jobj, "Datos del usuario cargados", STATE_SUCCESS);

    // const char *json_str = json_object_to_json_string(jobj);
    // puts(json_str);

    json_object_put(jobj); // Libera el objeto JSON
}

message_handler(actualizar_info_usuario)
{
    init_data_json();

    const char *nickname = get_string(get_array_idx(data, 0));
    const char *pais = get_string(get_array_idx(data, 1));
    const char *email = get_string(get_array_idx(data, 2));

    // Actualizar nickname
    VibeCast_ActualizarNickname(
        NULL,
        1,
        cast(const char *[], nickname),
        NULL);

    // Actualizar pais
    VibeCast_ActualizarPais(
        NULL,
        1,
        cast(const char *[], pais),
        NULL);

    // Actualizar email
    VibeCast_ActualizarEmail(
        NULL,
        1,
        cast(const char *[], email),
        NULL);

    VibeCast_SendText(id, HTTP_OK, "", "Datos actualizados", STATE_SUCCESS);
}

message_handler(actualizar_password)
{
    init_data_json();

    const char *currentPassword = get_string(get_array_idx(data, 0));
    const char *newPassword = get_string(get_array_idx(data, 1));
    const char *confirmPassword = get_string(get_array_idx(data, 2));

    char **msg = arg;

    bool success = VibeCast_ActualizarPassword(
        NULL,
        3,
        cast(const char *[],
             currentPassword,
             newPassword,
             confirmPassword),
        msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        "Actualización de contraseña",
        STATE_BOOL(success));
}

message_handler(activar_premium)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

/* ==== */

#include <ui/interfaces.h>

interfaz(activarPremium)
{
    time_t base = time(NULL);

    if (base == -1)
    {
        send_message("Error al obtener la fecha actual\n");
        return false;
    }

    if (usuario->plan != PLAN_PREMIUM)
    {
        usuario->plan = PLAN_PREMIUM;
        send_message("¡Plan Premium activado!");
    }
    else
    {
        base = usuario->caducidadPremium;
        send_message("¡Plan Premium renovado!");
    }

    usuario->caducidadPremium = base + 30 * 24 * 60 * 60; // Tiempo en segundos
    // La fecha es imprime como dia de la semana, mes, dia del mes, hora, minuto, segundo y año
    send_message("Caduca el: %s", ctime(&usuario->caducidadPremium));

    return true;
}

message_handler(activarPremium)
{

    init_data_json(); // Inicializa el JSON de entrada (aunque no hay argumentos)

    char **msg = arg;
    bool success = VibeCast_activarPremium(usuario, 0, NULL, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        success ? "Plan activado o renovado" : "Fallo al activar Premium",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}
//--------------------------------------------------------------------------------//

interfaz(desactivarPremium)
{
    if (usuario->plan == PLAN_PREMIUM)
    {
        usuario->plan = PLAN_FREEMIUM; // Cambiamos el plan del usuario a freemium
        usuario->caducidadPremium = 0; // Reseteamos la fecha de caducidad del plan premium
        send_message("¡Plan Premium desactivado!");
    }
    else
    {
        send_message("No estás en el plan Premium, no puedes desactivarlo.");
    }

    return true;
}

message_handler(desactivarPremium)
{
    init_data_json();

    char **msg = arg;
    bool success = VibeCast_desactivarPremium(usuario, 0, NULL, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        "Plan Freemium activado",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

//--------------------------------------------------------------------------------//

interfaz(esUsuarioPremium)
{

    time_t ahora = time(NULL);
    if (ahora == -1)
    {
        send_message("Error al obtener la fecha actual\n");
        return false;
    }

    if (usuario->caducidadPremium <= ahora)
    {
        send_message("Tu plan Premium ha caducado. Por favor, renueva tu suscripción\n");
        VibeCast_desactivarPremium(usuario, 0, NULL, NULL); // Desactiva el plan si ha caducado
        return false;                                       // Retorna false si el usuario no es premium
    }

    return usuario->plan == PLAN_PREMIUM; // Retorna true si el usuario es premium
}

message_handler(esUsuarioPremium)
{
    VibeCast_SendBool(
        id,
        HTTP_OK,
        usuario->plan == PLAN_PREMIUM,
        "",
        STATE_SUCCESS);
}

//--------------------------------------------------------------------------------//
interfaz(renovarPremium)
{
    VibeCast_activarPremium(usuario, 0, NULL, NULL); // Llama a la función para activar el plan premium
    send_message("Tu plan Premium ha sido renovado exitosamente\n");

    return true;
}

message_handler(renovarPremium)
{
    init_data_json();

    char **msg = arg;
    bool success = VibeCast_renovarPremium(usuario, 0, NULL, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        success ? "Renovación exitosa" : "Error al renovar",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}
