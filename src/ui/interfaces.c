#include <ui/interfaces.h>
#include <db/datos.h>
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
static Usuario *usuario = NULL;

// Cola de reproducción
static Cola cola_repr = {NULL};

/* Funciones principales */

#define log(...) \
    if (msg)     \
    *msg = mprintf(__VA_ARGS__)

interfaz(IsLoggedIn)
{
    return usuario ? true : false;
}

interfaz(IniciarSesion)
{
    const char *username = argv[0];
    const char *password = argv[1];

    printf("Datos recibidos en IniciarSesion():\n\tusername: %s\n\tpassword: %s\n", username, password);

    // TODO 1: Buscar el usuario en el árbol
    Nodo **ref = buscarNodo(ABB, &usuarios, username, cmpUsuarioConUsername);
    if (!ref)
    {
        log("Usuario no registrado.");
        return false;
    }

    usuario = (*ref)->value_ptr;
    if (strcmp(usuario->password, password))
    {
        log("Contraseña incorrecta.");
        usuario = NULL; // Regresar el puntero a NULL
        return false;
    }

    log("¡Inicio de sesión exitoso!");

    return true;
}

interfaz(CerrarSesion)
{
    return !(usuario = NULL);
}

// ======== Helper
select_handler(obtener_id_usuario)
{
    sscanf(argv[0], "%d", cast(int *, arg));
    return 0;
}
// ======== Helper

interfaz(CrearCuenta)
{
    if (argc != 6 || !argv)
    {
        log("%s: Número de parámetros (argc) incorrecto. Se esperaba 6.", __func__);
        return false;
    }

    for (int i = 0; i < argc; i++)
    {
        if (!argv[i])
        {
            log("Todos los campos son obligatorios.");
            return false;
        }
    }

    // email, username, password, confirmPassword, nickname, pais
    const char *email = argv[0];

    // Validar email
    if (!validar_email(email))
    {
        log("Dirección de correo no válida.");
        return false;
    }

    // Diseñar alguna forma de encontrar si el correo ya está registrado (sin depender de sqlite)

    const char *username = argv[1];
    if (buscarNodo(ABB, &usuarios, username, cmpUsuarioConUsername))
    {
        log("Usuario ya existente.");
        return false;
    }

    const char *password = argv[2];
    const char *confirmPassword = argv[3];
    if (strcmp(password, confirmPassword))
    {
        log("Las contraseñas no coinciden.");
        return false;
    }

    const char *nickname = argv[4];
    const char *pais = argv[5];

    char *datos =
        mprintf(
            stringify("%s", "%s", "%s", "%s", "%s"),
            username, email, password, nickname, pais);

    if (!nuevo_registro(
            "Usuarios",
            "username, email, password, nickname, pais",
            datos, NULL))
    {
        log("Error al registrar datos en la base de datos.");
        free(datos);
        return false;
    }

    usuario =
        alloc(
            Usuario,
            &cast(
                Usuario,
                .id = 0, // Hasta nuevo aviso

                .username = mprintf(username),
                .email = mprintf(email),
                .password = mprintf(password),
                .nickname = mprintf(nickname),
                .pais = mprintf(pais),
                .plan = PLAN_FREEMIUM,

                .artista = NULL,

                .amigos.head = NULL,
                .playlists.head = NULL,

                .historial.reproducciones.top = NULL,
                .historial.tiempoEscuchado = 0,
                .historial.cantidadAnuncios = 0));

    // Insertar usuario en el árbol

    if (!insertarNodo(ABB, &usuarios, usuario, cmpUsuarios))
    {
        log("No se pudo insertar el usuario en el ABB. Reinicie la aplicación e intente iniciar sesión.");

        free(usuario->username);
        free(usuario->email);
        free(usuario->password);
        free(usuario->nickname);
        free(usuario->pais);
        usuario = NULL;

        return false;
    }

    // Establecer el id del usuario
    if (!obtener_registros(
            "Usuarios ORDER BY id DESC LIMIT 1",
            "id", NULL, obtener_id_usuario, &usuario->id, msg))
    {
        puts(*msg);
        free_errmsg(*msg);
        log("Error al obtener el identificador de usuario. Reinicia la aplicación e intenta iniciar sesión.");
        return false;
    }

    printf("El nuevo usuario tiene id = %d\n", usuario->id);

    // Si se quiere que una vez registrado ya inicie sesión
    usuario = NULL; // Comentar esta linea

    log("Cuenta creada correctamente.");

    return true;
}
