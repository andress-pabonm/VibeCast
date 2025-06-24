#include <ui/conexiones.h>
#include <ui/interfaces.h>
#include <stdio.h>
#include <string.h>

#include <json.h>               // Para manejar los mensajes enviados desde JS
#define PCRE2_CODE_UNIT_WIDTH 8 // Para establecer que se manejan caracteres de 8 bits
#include <pcre2.h>              // Para expresiones regulares

/* ======== Utilidades ======== */

// ========
// JSON-C
// ========

// Para obtener el JSON dentro de un string
#define get_json json_tokener_parse

// Para obtener el JSON dentro de un array
#define get_array_idx json_object_array_get_idx

// Para obtener un campo de un JSON
static inline struct json_object *get_field(struct json_object *src, const char *field)
{
    struct json_object *dest;
    json_object_object_get_ex(src, field, &dest);
    return dest;
}

// Para convertir un string a JSON
#define init_json(dest, src) struct json_object *dest = src

// Para inicializar (data) con el JSON obtenido de (req)
#define init_data_json() init_json(data, get_json(req))

// Para obtener el string de un JSON dado
#define get_string(src) mprintf("%s", json_object_get_string(src))

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

message_handler(test)
{
    puts(req);
    send_message(0, "test done");
}

message_handler(is_logged_in)
{
    if (usuario)
        send_message(0, TRUE);
    else
        send_message(0, FALSE);
}

message_handler(iniciar_sesion)
{
    if (usuario)
    {
        send_message(0, STATUS_SUCCESS);
        return;
    }

    // Inicializa (data) con el JSON obtenido desde (req)
    init_data_json();

    // TODO 1: Obtener los datos de entrada
    char *username = get_string(get_array_idx(data, 0));

    // TODO 2: Buscar username en los datos
    Nodo **ref = buscarNodo(ABB, &usuarios, username, cmpUsuarioConUsername);
    if (!ref)
    {
        puts("No se encontro el username");
        free(username);
        send_message(0, STATUS_FAILURE);
        return;
    }

    free(username);
    char *password = get_string(get_array_idx(data, 1));

    // TODO 3: Verificar que la contraseña sea correcta
    usuario = (*ref)->value_ptr;
    if (strcmp(usuario->password, password))
    {
        printf("La password no coincide: %s != %s\n", usuario->password, password);
        free(password);
        usuario = NULL;
        send_message(0, STATUS_FAILURE);
        return;
    }

    free(password);

    send_message(0, STATUS_SUCCESS);
}

message_handler(cerrar_sesion)
{
    if (usuario)
        usuario = NULL;

    send_message(0, STATUS_SUCCESS);
}

message_handler(crear_cuenta)
{
    if (usuario)
    {
        send_message(0, "Hay una sesión iniciada.");
        return;
    }

    init_data_json();

    // nickname, pais, username, email, password, passwordConfirmation

    char *username = get_string(get_array_idx(data, 2));
    if (buscarNodo(ABB, &usuarios, username, cmpUsuarioConUsername))
    {
        free(username);
        send_message(0, "El nombre de usuario ya existe.");
        return;
    }

    char *email = get_string(get_array_idx(data, 3));

    if (!validar_email(email))
    {
        free(username);
        free(email);
        send_message(0, "Correo electrónico no válido.");
        return;
    }

    char *condition = mprintf(stringify(email = "%s"), email);
    if (contar_registros("Usuarios", condition, NULL))
    {
        free(username);
        free(email);
        free(condition);
        send_message(0, "El correo ya está registrado.");
        return;
    }

    free(condition);

    char *password = get_string(get_array_idx(data, 4));
    char *confirmPassword = get_string(get_array_idx(data, 5));

    if (strcmp(password, confirmPassword))
    {
        free(username);
        free(email);
        free(password);
        free(confirmPassword);
        send_message(0, "Las contraseñas no coinciden.");
        return;
    }

    free(confirmPassword);

    char *nickname = get_string(get_array_idx(data, 0));
    char *pais = get_string(get_array_idx(data, 1));

    // TODO: Verificar que nickname y pais no sean nulos o sean un texto vacio ("")

    char *values = mprintf(stringify("%s", "%s", "%s", "%s", "%s"), email, password, username, nickname, pais);

    if (!nuevo_registro("Usuarios", "email, password, username, nickname, pais", values, NULL))
    {
        free(email);
        free(password);
        free(username);
        free(nickname);
        free(pais);
        free(values);
        send_message(0, "Error al crear cuenta.");
        return;
    }

    free(values);

    Usuario user = {
        .email = email,
        .password = password,
        .username = username,
        .nickname = nickname,
        .pais = pais,
        .plan = PLAN_FREEMIUM,

        .amigos.head = NULL,
        .playlists.head = NULL,

        .historial.reproducciones.top = NULL,
        .historial.tiempoEscuchado = 0,
        .historial.cantidadAnuncios = 0,

        .artista = NULL};

    Usuario *ptr_user = alloc(Usuario, &user);

    if (!ptr_user || !insertarNodo(ABB, &usuarios, ptr_user, cmpUsuarios))
    {
        free(email);
        free(password);
        free(username);
        free(nickname);
        free(pais);
        send_message(0, "Error inesperado.");
        return;
    }

    printf("Se registro el username: %s\n", ptr_user->username);

    send_message(0, STATUS_SUCCESS);
}
