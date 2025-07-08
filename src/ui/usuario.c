#include <ui/interfaces.h>
#include <db/dbmgr.h>
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

/* ========== Usuario actual ========== */

Usuario *usuario = NULL;

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

message_handler(is_logged_in)
{
    VibeCast_SendBool(id, usuario ? HTTP_OK : HTTP_UNAUTHORIZED, usuario != NULL, NULL, STATE_SUCCESS);
}

message_handler(cerrar_sesion)
{
    usuario = NULL;
    VibeCast_SendBool(id, HTTP_OK, true, "Sesión cerrada", STATE_SUCCESS);
}

/* ========== Funciones de interfaz ========== */

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
        usuario = NULL;
        send_message("Contraseña incorrecta");
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
    bool ok = VibeCast_IniciarSesion(NULL, 2, cast(const char *[], username, password), msg);
    VibeCast_SendBool(id, ok ? HTTP_OK : HTTP_BAD_REQUEST, ok, *msg, ok ? STATE_SUCCESS : STATE_FAILURE);
    freem(*msg);
    *msg = NULL;
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
    search_email_arg_t arg = {&emailRepetido, email};
    forEachInABB_InOrder(usuarios, check_email_repetido, &arg);
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

    usuario = newUsuario();
    usuario->username = asprintf(username);
    usuario->email = asprintf(email);
    usuario->password = asprintf(password);
    usuario->nickname = asprintf(nickname);
    usuario->pais = asprintf(pais);

    char *cond = asprintf(stringify(username = "%s"), username);
    obtener_registros("Usuarios", "id", cond, obtener_id_usuario, &usuario->id, NULL);
    freem(cond);

    insertValueInABB(usuarios, usuario);
    usuario = NULL;

    send_message("Cuenta creada exitosamente");
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

/*
    FALTANTE: Se deben completar las funciones VibeCast_ActualizarUsername, _Nickname, etc.
    También actualizar correctamente desde actualizar_usuario llamando a las 4 funciones.
*/
