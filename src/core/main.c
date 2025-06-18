#include <VibeCastConfig.h>

#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

#include <utils/utils.h>
#include <ui/interfaces.h>

#include <webview/webview.h> // Para la interfaz gráfica
#include <stdio.h>           // Mientras se prueba la aplicación.

#define stringify(expr) #expr
#define JSON(...) stringify({__VA_ARGS__})

void handle_message(const char *id, const char *req, void *arg);

static webview_t w = NULL;

AppResult AppInit(void **appstate, int argc, char *argv[])
{
    if (!func(LoadData))
    {
        puts("No se pudo cargar los datos.");
        return APP_FAILURE;
    }

    w = webview_create(1, NULL); // Crear la ventan
    if (!w)
    {
        puts("No se pudo crear la ventana.");
        return APP_FAILURE;
    }

    // Configuraciones iniciales de la ventana
    webview_set_title(w, "VibeCast");                  // Establecer el título de la ventana
    webview_set_size(w, 480, 640, WEBVIEW_HINT_FIXED); // Establecer el tamaño de la ventana

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

void handle_message(const char *id, const char *req, void *arg)
{
    printf("req: %s\n", req);

    bool status = false;

    switch (*cast(Interfaz *, arg))
    {
    case LOGIN:
        status = iniciar_sesion(req);
        break;

    default:
        puts("Interfaz no implementada.");
    }

    char response[256];

    if (status)
        strcpy(response, JSON("status" : "OK"));
    else
        strcpy(response, JSON("status" : "FAIL"));

    webview_return(w, id, 0, response);
}
