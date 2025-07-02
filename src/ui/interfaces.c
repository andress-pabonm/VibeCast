#include <ui/interfaces.h>
#include <utils/utils.h>
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

void free_re()
{
    pcre2_code_free(get_email_re());
}

static bool validar_email(const char *email)
{
    static pcre2_code *re = NULL;

    if (!re)
    {
        re = get_email_re();
        if (!re)
            return false;
    }

    PCRE2_SPTR subject = (PCRE2_SPTR)email;
    size_t subject_length = strlen(email);

    // Crear el "match data" para el resultado
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // Ejecutar la coincidencia
    int rc = pcre2_match(
        get_email_re(), // El patrón compilado
        email,          // El texto a verificar
        strlen(email),  // Longitud del texto
        0,              // Offset inicial
        0,              // Opciones
        match_data,     // Resultado
        NULL);          // Contexto de match (opcional)

    // Liberar memoria
    pcre2_match_data_free(match_data);

    // Validar el resultado
    return rc >= 0;
}

/* ======== Funcionalidad principal de las interfaces ======== */

// Puntero al usuario que ha iniciado sesión
Usuario *usuario = NULL;

// Cola de reproducción
Cola cola_repr = NULL;

/* Funciones principales */

#define send_message(...) \
    if (msg)              \
    *msg = asprintf(__VA_ARGS__)

interfaz(IsLoggedIn)
{
    return usuario ? true : false;
}

interfaz(IniciarSesion)
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
        usuario = NULL;
        return false;
    }

    send_message("Inicio de sesión exitoso");

    return true;
}

interfaz(CerrarSesion)
{
    return !(usuario = NULL);
}

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

static new_operfn(checkEmailRepetido)
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

interfaz(CrearCuenta)
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

    forEachInABB_InOrder(usuarios, checkEmailRepetido, &wrapper_arg);

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
