#include <VibeCastConfig.h>

#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

#include <db/dbmgr.h>
#include <utils/utils.h>
#include <ui/interfaces.h>

#include <webview/webview.h> // Para la interfaz gráfica
#include <json.h>            // Para recibir las peticiones desde JS
#include <stdio.h>           // Mientras se prueba la aplicación.
#include <string.h>

// Definir tamaños mínimos y máximos
#define MIN_WIDTH 320
#define MIN_HEIGHT 480
#define INIT_WIDTH 480
#define INIT_HEIGHT 640

#define JSON(...) stringify({__VA_ARGS__})

typedef void (*message_handler_t)(const char *id, const char *req, void *arg);
#define new_message_handler(name) void name(const char *id, const char *req, void *arg)

new_message_handler(handle_message);

static webview_t w = NULL;

AppResult AppInit(void **appstate, int argc, char *argv[])
{
    if (!func(InitDB, ":memory:", NULL))
    {
        puts("Hubo un error al inicializar la base de datos.");
        return APP_FAILURE;
    }

    // Aquí se carga un registro de pruebo
    if (nuevo_registro(
            "Usuarios",
            "email, password, username, nickname, pais, plan, nombre_artista",
            stringify(
                "ejemplo@vibecast.com",
                "ejemploPassword",
                "ejemploUsername",
                "ejemploNickname",
                "EC",
                0,
                "ejemploArtista"),
            NULL))
    {
        puts("Registro de ejemplo exitoso.");
    }

    if (!func(LoadData))
    {
        puts("Hubo un error al cargar los datos.");
        return APP_FAILURE;
    }

    w = webview_create(1, NULL); // Crear la ventana
    if (!w)
    {
        puts("No se pudo crear la ventana.");
        return APP_FAILURE;
    }

    // Configuraciones iniciales de la ventana
    webview_set_title(w, "VibeCast");                 // Establecer el título de la ventana
    webview_set_size(w, 480, 640, WEBVIEW_HINT_NONE); // Establecer el tamaño de la ventana
    // Configurar límites de tamaño
    // webview_set_minsize(w, MIN_WIDTH, MIN_HEIGHT); no funciona xd

    Interfaz interfaz = LOGIN;
    *appstate = alloc(Interfaz, &interfaz);
    if (!*appstate)
    {
        puts("Fallo al inicializar la interfaz.");
        return APP_FAILURE;
    }

    // Enlazar funciones para controlar la interfaz gráfica
    webview_bind(w, "enviarMensaje", handle_message, *appstate);

    // Iniciar la aplicación en esta interfaz
    webview_navigate(w, INTERFAZ("Login/index.html"));

    puts("Aplicación inicializada correctamente.");

    return APP_CONTINUE; // Continuar con la ejecución
}

AppResult AppIterate(void *appstate)
{
    // Ejecutar la ventana (bucle principal de la ventana)
    webview_run(w);

    return APP_SUCCESS; // Finalizar ejecución
}

void AppQuit(void *appstate, AppResult appresult)
{
    webview_destroy(w); // Destruir la ventana

    puts("Ejecución finalizada");
}

new_select_handler(comprobarUsuario)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", fields[i], argv[i]);
    }

    return 0;
}

new_message_handler(handle_message)
{
    printf("req: %s\n", req);

    // TODO 1: Obtener el tipo de petición que se está haciendo
    struct json_object *array = json_tokener_parse(req);
    struct json_object *json_str_obj = json_object_array_get_idx(array, 0);
    const char *json_inner_str = json_object_get_string(json_str_obj);
    struct json_object *parsed_json = json_tokener_parse(json_inner_str);

    struct json_object *type, *data;

    json_object_object_get_ex(parsed_json, "type", &type);
    json_object_object_get_ex(parsed_json, "data", &data);

    const char *type_str = json_object_get_string(type);
    printf("type: %s\n", type_str);

    // TODO 2: Según el tipo de petición, invocar a las diferentes funciones
    if (!strcmp(type_str, "login"))
    {
        puts("Estás intentando iniciar sesión.");

        struct json_object *email, *password;

        json_object_object_get_ex(data, "email", &email);
        json_object_object_get_ex(data, "password", &password);

        const char *email_str = json_object_get_string(email);
        const char *password_str = json_object_get_string(password);

        printf("%s %s\n", email_str, password_str);
        
        char condition[256];
        snprintf(condition, sizeof(condition),
                 stringify(email = "%s"), email_str);

        obtener_registros(
            "*",
            "Usuarios",
            condition,
            comprobarUsuario,
            NULL,
            NULL);
    }

    // TODO 3: Enviar un mensaje de respuesta a JS

    webview_return(w, id, 0, JSON("status" : "OK"));
}
