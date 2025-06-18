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

// Definir tamaños mínimos y máximos
#define MIN_WIDTH 320 
#define MIN_HEIGHT 480
#define INIT_WIDTH 480
#define INIT_HEIGHT 640

#define stringify(expr) #expr
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
    webview_set_title(w, "VibeCast");                  // Establecer el título de la ventana
    webview_set_size(w, 480, 640, WEBVIEW_HINT_NONE); // Establecer el tamaño de la ventana
    // Configurar límites de tamaño
    //webview_set_minsize(w, MIN_WIDTH, MIN_HEIGHT); no funciona xd

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

new_message_handler(handle_message)
{
    printf("req: %s\n", req);

    // TODO 1: Obtener el tipo de petición que se está haciendo
    // TODO 2: Según el tipo de petición, invocar a las diferentes funciones
    // TODO 3: Enviar un mensaje de respuesta a JS

    webview_return(w, id, 0, JSON("status" : "OK"));
}
