#include <ui/interfaces.h>
#include <db/datos.h>
#include <db/dbmgr.h>
#include <utils/utils.h>

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
// ======== Helper

interfaz(CrearCuenta)
{
    return true;
}
