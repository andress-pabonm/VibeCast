#include <ui/interfaces.h>
#include <db/dbmgr.h>

#define PCRE2_CODE_UNIT_WIDTH 8 // Para establecer que se manejan caracteres de 8 bits
#include <pcre2.h>              // Para expresiones regulares

/* ======== Utilidades ======== */

// ========
// PCRE2
// ========

static pcre2_code *get_email_re()
{
    static pcre2_code *re = NULL;

    if (re)
        return re;

    int errornumber;
    PCRE2_SIZE erroroffset;

    // Expresión regular para email
    PCRE2_SPTR pattern = (PCRE2_SPTR) "^([a-zA-Z0-9]+([._-][a-zA-Z0-9]+)*)@([a-zA-Z0-9]+(-[a-zA-Z0-9]+)*)(\\.[a-zA-Z]{2,})+$";

    // Compilar la expresión regular
    re = pcre2_compile(
        pattern,               // El patrón
        PCRE2_ZERO_TERMINATED, // El patrón es un string terminado en '\0'
        0,                     // Opciones
        &errornumber,          // Código de error si hay uno
        &erroroffset,          // Posición del error en el patrón
        NULL);                 // Contexto de compilación (opcional)

    if (!re)
    {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        fprintf(stderr, "Error al compilar la expresión regular: %s\n", buffer);
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

    // Expresión regular para email
    PCRE2_SPTR pattern = (PCRE2_SPTR) "^(?=.*[A-Z])(?=.*[a-z])(?=.*\d)(?=.*[^A-Za-z0-9]).{8,}$";

    // Compilar la expresión regular
    re = pcre2_compile(
        pattern,               // El patrón
        PCRE2_ZERO_TERMINATED, // El patrón es un string terminado en '\0'
        0,                     // Opciones
        &errornumber,          // Código de error si hay uno
        &erroroffset,          // Posición del error en el patrón
        NULL);                 // Contexto de compilación (opcional)

    if (!re)
    {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        fprintf(stderr, "Error al compilar la expresión regular: %s\n", buffer);
    }

    return re;
}

void free_re()
{
    pcre2_code_free(get_email_re());
    pcre2_code_free(get_password_re());
}

static bool validar_email(const char *email)
{
    static pcre2_code *re = NULL;

    if (!email)
        return false;

    if (!re)
    {
        re = get_email_re();
        if (!re)
            return false;
    }

    // Crear el "match data" para el resultado
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // Ejecutar la coincidencia
    int rc = pcre2_match(
        re,            // El patrón compilado
        email,         // El texto a verificar
        strlen(email), // Longitud del texto
        0,             // Offset inicial
        0,             // Opciones
        match_data,    // Resultado
        NULL);         // Contexto de match (opcional)

    // Liberar memoria
    pcre2_match_data_free(match_data);

    // Validar el resultado
    return rc >= 0;
}

static bool validar_password(const char *password)
{
    static pcre2_code *re = NULL;

    if (!password)
        return false;

    if (!re)
    {
        re = get_password_re();
        if (!re)
            return false;
    }

    // Crear el "match data" para el resultado
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // Ejecutar la coincidencia
    int rc = pcre2_match(
        re,               // El patrón compilado
        password,         // El texto a verificar
        strlen(password), // Longitud del texto
        0,                // Offset inicial
        0,                // Opciones
        match_data,       // Resultado
        NULL);            // Contexto de match (opcional)

    // Liberar memoria
    pcre2_match_data_free(match_data);

    // Validar el resultado
    return rc >= 0;
}

/* ================================================================ */

// Puntero al usuario que ha iniciado sesión
Usuario *usuario = NULL;

/* ================================================================ */

message_handler(is_logged_in)
{
    bool result = usuario ? true : false;
    VibeCast_SendBool(id, result ? HTTP_OK : HTTP_UNAUTHORIZED, result, NULL, STATE_SUCCESS);
}

/* ================================================================ */

static interfaz(IniciarSesion)
{
    const char *username = argv[0];
    const char *password = argv[1];

    usuario = searchValueInABB(usuarios, username, cmpUsuarioConUsername);
    if (!usuario)
    {
        send_message("Usuario no registrado");
        return false;
    }

    if (strcmp(usuario->password, password))
    {
        send_message("Contraseña incorrecta");
        usuario = NULL; // Se cierra la sesión
        return false;
    }

    send_message("Inicio de sesión exitoso");

    return true;
}

message_handler(iniciar_sesion)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0));
    const char *password = get_string(get_array_idx(data, 1));

    char **msg = arg;
    bool success = VibeCast_IniciarSesion(NULL, 2, cast(const char *[], username, password), msg);

    VibeCast_SendBool(id, success ? HTTP_OK : HTTP_BAD_REQUEST, success, *msg, success ? STATE_SUCCESS : STATE_FAILURE);

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

/* ================================================================ */

message_handler(cerrar_sesion)
{
    bool success = !(usuario = NULL);
    VibeCast_SendBool(id, success ? HTTP_OK : HTTP_BAD_REQUEST, success,
                      success ? "Sesión cerrada" : "No se pudo cerrar sesión",
                      success ? STATE_SUCCESS : STATE_FAILURE);
}

/* ================================================================ */

// ======== Helper
static select_handler(obtener_id_usuario)
{
    sscanf(argv[0], "%d", cast(int *, arg));
    return 0;
}

typedef struct
{
    bool *check;
    const char *email;
} search_email_arg_t;

static new_operfn(check_email_repetido)
{
    search_email_arg_t *wrapper_arg = arg;

    if (*wrapper_arg->check)
        return FOREACH_BREAK;

    Usuario *u = val;

    if (strcmp(u->email, wrapper_arg->email))
        return FOREACH_CONTINUE;

    *wrapper_arg->check = true;
    return FOREACH_BREAK;
}
// ======== Helper

static interfaz(CrearCuenta)
{
    for (int i = 0; i < argc; i++)
    {
        if (!argv[i])
        {
            send_message("Todos los campos son obligatorios");
            return false;
        }
    }

    const char *email = argv[0];

    // Verificar que el correo es válido

    if (!validar_email(email))
    {
        send_message("Correo no válido");
        return false;
    }

    // Verificar que el email no esté registrado

    bool emailRepetido = false;

    search_email_arg_t wrapper_arg = {
        .check = &emailRepetido,
        .email = email,
    };

    forEachInABB_InOrder(usuarios, check_email_repetido, &wrapper_arg);

    if (emailRepetido)
    {
        send_message("El email ya está registrado.");
        return false;
    }

    const char *username = argv[1];

    // Verificar que el username no esté registrado

    if (searchValueInABB(usuarios, username, cmpUsuarioConUsername))
    {
        send_message("El username ya existe.");
        return false;
    }

    const char *password = argv[2];

    if (!validar_password(password))
    {
        send_message("La contraseña debe tener al menos 8 caracteres, 1 mayúscula, 1 minúscula, 1 número y 1 caracter especial.");
        return false;
    }

    const char *confirmPassword = argv[3];

    // Verificar que las contraseñas coincidan

    if (strcmp(password, confirmPassword))
    {
        send_message("Las contraseñas no coinciden.");
        return false;
    }

    const char *nickname = argv[4];
    const char *pais = argv[5];

    // Se puede continuar con el registro

    // Empaquetar los datos en un único string
    char *datos = asprintf(
        stringify("%s", "%s", "%s", "%s", "%s"),
        username, email, password, nickname, pais);

    // Registrar en la base de datos
    nuevo_registro(
        "Usuarios",
        "username, email, password, nickname, pais",
        datos, NULL);

    // Crear el nuevo usuario e insertarlo en el árbol

    usuario = newUsuario();

    usuario->username = asprintf(username);
    usuario->email = asprintf(email);
    usuario->password = asprintf(password);
    usuario->nickname = asprintf(nickname);
    usuario->pais = asprintf(pais);

    // Obtener el id del nuevo usuario

    char *condition = asprintf(
        stringify(username = "%s"),
        username);

    obtener_registros(
        "Usuarios", "id", condition,
        obtener_id_usuario,
        &usuario->id,
        NULL);

    freem(condition);

    insertValueInABB(usuarios, usuario);

    usuario = NULL; // Para indicar que no queda iniciado sesión

    send_message("Cuenta creada exitosamente.");

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
    bool success = VibeCast_CrearCuenta(NULL, 6, datos, msg);

    VibeCast_SendBool(id, success ? HTTP_CREATED : HTTP_BAD_REQUEST, success, *msg, success ? STATE_SUCCESS : STATE_FAILURE);

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

/* ================================================================ */

interfaz(ActualizarUsername)
{
    // arg, argc, argv, msg

    const char *username = argv[0];

    if (searchValueInABB(usuarios, username, cmpUsuarioConUsername))
    {
        send_message("El usuario ya existe");
        return false;
    }

    // Quitarlo del ABB y volverlo a insertar
    deleteValueInABB(usuarios, usuario->username, cmpUsuarioConUsername);

    // Actualizo el username
    freem(usuario->username);
    usuario->username = asprintf(username);

    // Volverlo a insertar en al ABB
    insertValueInABB(usuarios, usuario);

    return true;
}

interfaz(ActualizarNickname)
{
    const char *nickname = argv[0];

    // Validación que no sea null

    freem(usuario->nickname);
    usuario->nickname = asprintf(nickname);

    return true;
}

interfaz(ActualizarEmail)
{
}

interfaz(ActualizarPassword)
{
}

interfaz(ActualizarPais)
{
}
message_handler(actualizar_usuario)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0)); // data[0]
    const char *email = get_string(get_array_idx(data, 1));    // data[1]
    const char *nickname = get_string(get_array_idx(data, 2)); // data[2]
    const char *pais = get_string(get_array_idx(data, 3));     // data[3]

    const char *argv[] = {username};

    char **msg = arg;
    if (!VibeCast_ActualizarUsername(usuario, 1, argv, msg)){
        VibeCast_SendText(id, HTTP_OK, *msg, "",STATE_FAILURE);
        return ;
    }
}