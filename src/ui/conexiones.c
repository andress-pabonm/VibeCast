#include <ui/conexiones.h>
#include <ui/interfaces.h>
#include <ui/gui.h>
#include <utils/utils.h>

#include <json.h> // Para manejar los mensajes enviados desde JS

/**
 * Macro para declarar y definir fácilmente funciones de tipo (message_handler_t)
 * @param name: Nombre de la función
 */
#define message_handler(name) void name(const char *id, const char *req, void *arg)

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
#define get_string(src) json_object_get_string(src)

/* ======== Funcionalidad principal de las interfaces ======== */

#define TRUE "1"
#define FALSE "0"
#define STATUS_SUCCESS TRUE
#define STATUS_FAILURE FALSE

static message_handler(test)
{
    puts(req);
    send_message(0, STATUS_SUCCESS);
}

static message_handler(is_logged_in)
{
    if (func(IsLoggedIn, 0, NULL, NULL))
        send_message(0, TRUE);
    else
        send_message(0, FALSE);
}

static message_handler(iniciar_sesion)
{
    // Inicializa (data) con el JSON obtenido desde (req)
    init_data_json();

    // Obtener los datos enviados
    const char *username = get_string(get_array_idx(data, 0));
    const char *password = get_string(get_array_idx(data, 1));

    printf("Datos recibidos en iniciar_sesion():\n\tusername: %s\n\tpassword: %s\n", username, password);

    // Llamar a la función para iniciar sesión
    char **msg = arg;
    if (func(IniciarSesion, 2,
             cast(const char *[],
                  username, password),
             msg))
        send_message(0, STATUS_SUCCESS);
    else
        send_message(0, STATUS_FAILURE);

    // Mostrar mensaje y liberar memoria.
    puts(*msg);
    free(*msg);
    *msg = NULL;
}

static message_handler(cerrar_sesion)
{
    if (func(CerrarSesion, 0, NULL, NULL))
        send_message(0, STATUS_SUCCESS);
    else
        send_message(0, STATUS_FAILURE);
}

static message_handler(crear_cuenta)
{
    init_data_json();

    // Recibido: nickname, pais, username, email, password, confirmPassword
    
    const char *nickname = get_string(get_array_idx(data, 0));
    const char *pais = get_string(get_array_idx(data, 1));
    const char *username = get_string(get_array_idx(data, 2));
    const char *email = get_string(get_array_idx(data, 3));
    const char *password = get_string(get_array_idx(data, 4));
    const char *confirmPassword = get_string(get_array_idx(data, 5));
    
    // Enviado: email, username, password, confirmPassword, nickname, pais

    const char *datos[] = {
        email,
        username,
        password,
        confirmPassword,
        nickname,
        pais};

    char **msg = arg;
    if (func(CrearCuenta, 6, datos, msg))
        send_message(0, STATUS_SUCCESS);
    else
        send_message(0, *msg);

    puts(*msg);
    free(*msg);
    *msg = NULL;
}

/* ======== Para conectar las interfaces ======== */

bool func(InitBindings)
{
    char *msg = NULL;

    bind_fn(test, &msg);

    bind_fn(is_logged_in, &msg);
    bind_fn(iniciar_sesion, &msg);
    bind_fn(cerrar_sesion, &msg);
    bind_fn(crear_cuenta, &msg);

    return true;
}
