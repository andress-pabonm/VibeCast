#include <ui/interfaces.h>
#include <time.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

/* ================================================================ */
// DEFINICIÓN DE ESTRUCTURAS INTERNAS
/* ================================================================ */

typedef struct
{
    bool *check;
    const char *email;
} search_email_arg_t;

/* ================================================================ */
// DEFINICIÓN DE ESTRUCTURAS INTERNAS
/* ================================================================ */

static interfaz(CrearCuenta);

static interfaz(EliminarCuenta);

static interfaz(ObtenerInfoUsuario);

static interfaz(ActualizarInfoUsuario);
static custom_interface(ActualizarNickname, const char *nickname);
static custom_interface(ActualizarPais, const char *pais);
static custom_interface(ActualizarUsername, const char *username);
static custom_interface(ActualizarEmail, const char *email);
static new_operfn(check_email_repetido);

static interfaz(ActualizarPassword);

static interfaz(ActivarPremium);

static pcre2_code *get_email_re();
static pcre2_code *get_password_re();
static bool validar_email(const char *email);
static bool validar_password(const char *password);

/* ================================================================ */
// BLOQUE: crear_cuenta -
/* ================================================================ */

message_handler(crear_cuenta)
{
    init_data_json();

    const char *email = get_string(get_array_idx(data, 0));
    const char *username = get_string(get_array_idx(data, 1));
    const char *password = get_string(get_array_idx(data, 2));
    const char *confirmPassowrd = get_string(get_array_idx(data, 3));
    const char *nickname = get_string(get_array_idx(data, 4));
    const char *pais = get_string(get_array_idx(data, 5));

    int argc = 6;
    const char *argv[] = {email, username, password, confirmPassowrd, nickname, pais};
    char **msg = arg;

    bool success = VibeCast_CrearCuenta(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

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

/* ================================================================ */
// BLOQUE: eliminar_cuenta -
/* ================================================================ */

message_handler(eliminar_cuenta)
{
    char **msg = arg;

    bool success = VibeCast_EliminarCuenta(NULL, 0, NULL, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

static interfaz(EliminarCuenta)
{
    return true;
}

/* ================================================================ */
// BLOQUE: obtener_info_usuario -
/* ================================================================ */

message_handler(obtener_info_usuario)
{
    json_object *jobj = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_ObtenerInfoUsuario(jobj, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, jobj, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(jobj);
}

static interfaz(ObtenerInfoUsuario)
{
    json_object_object_add(arg, "name", json_object_new_string(usuario->nickname));
    json_object_object_add(arg, "username", json_object_new_string(usuario->username));
    json_object_object_add(arg, "email", json_object_new_string(usuario->email));
    json_object_object_add(arg, "country", json_object_new_string(usuario->pais));
    json_object_object_add(arg, "isArtist", json_object_new_boolean(usuario->artista != NULL));

    // Subobjeto de suscripción
    json_object *subscription = json_object_new_object();

    const char *tipo_plan = (usuario->plan == PLAN_PREMIUM) ? "premium" : "freemium";
    json_object_object_add(subscription, "type", json_object_new_string(tipo_plan));
    json_object_object_add(subscription, "autoRenewal", json_object_new_boolean(true)); // Asumido

    // Fecha de expiración: 180 días desde hoy
    time_t now = time(NULL);
    now += 180 * 24 * 60 * 60;
    struct tm *exp_date = localtime(&now);
    char fecha_exp[11];
    strftime(fecha_exp, sizeof(fecha_exp), "%Y-%m-%d", exp_date);
    json_object_object_add(subscription, "expiration", json_object_new_string(fecha_exp));

    json_object_object_add(arg, "subscription", subscription);

    send_message("Perfil de usuario cargado.");
    return true;
}

/* ================================================================ */
// BLOQUE: actualizar_info_usuario -
/* ================================================================ */

message_handler(actualizar_info_usuario)
{
    init_data_json();

    const char *nickname = get_string(get_array_idx(data, 0));
    const char *pais = get_string(get_array_idx(data, 1));
    const char *username = get_string(get_array_idx(data, 2));
    const char *email = get_string(get_array_idx(data, 3));

    int argc = 4;
    const char *argv[] = {nickname, pais, username, email};
    char **msg = arg;

    bool success = VibeCast_ActualizarInfoUsuario(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(ActualizarInfoUsuario)
{
    const char *nickname = argv[0];
    const char *pais = argv[1];
    const char *username = argv[2];
    const char *email = argv[3];

    char *msgs[4];

    VibeCast_ActualizarNickname(NULL, nickname, NULL);
    VibeCast_ActualizarPais(NULL, pais, NULL);
    VibeCast_ActualizarUsername(NULL, username, NULL);
    VibeCast_ActualizarEmail(NULL, email, NULL);

    send_message("Usuario actualizado");

    return true;
}

static custom_interface(ActualizarNickname, const char *nickname)
{
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

static custom_interface(ActualizarPais, const char *pais)
{
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

static custom_interface(ActualizarUsername, const char *username)
{
    // Validar que el campo no sea nulo
    if (!username || !*username)
    {
        send_message("El username no puede estar vacío");
        return false;
    }

    if (!strcmp(usuario->username, username))
    {
        send_message("El username no puede ser igual");
        return false;
    }

    if (searchValueInABB(usuarios, username, cmpUsuarioConUsername))
    {
        send_message("El nombre del usuario ya existe");
        return false;
    }

    deleteValueInABB(usuarios, usuario->username, cmpUsuarioConUsername);

    freem(usuario->username);

    usuario->username = asprintf(username);

    insertValueInABB(usuarios, usuario);

    send_message("Username actualizado");

    return true;
}

static custom_interface(ActualizarEmail, const char *email)
{
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

/* ================================================================ */
// BLOQUE: actualizar_password -
/* ================================================================ */

message_handler(actualizar_password)
{
    init_data_json();

    const char *currentPassword = get_string(get_array_idx(data, 0));
    const char *newPassword = get_string(get_array_idx(data, 1));
    const char *confirmPassword = get_string(get_array_idx(data, 2));

    int argc = 3;
    const char *argv[] = {currentPassword, newPassword, confirmPassword};
    char **msg = arg;

    bool success = VibeCast_ActualizarPassword(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(ActualizarPassword)
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

/* ================================================================ */
// BLOQUE: activar_premium -
/* ================================================================ */

message_handler(activar_premium)
{
    init_data_json();

    const char *duracion = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {duracion};

    char **msg = arg;

    bool success = VibeCast_ActivarPremium(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(ActivarPremium)
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

/* ================================================================ */
// DEFINICIÓN DE FUNCIONES AUXILIARES
/* ================================================================ */

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
