#include <VibeCastConfig.h>

#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

#include <webview/webview.h> // Para la interfaz gráfica

#include <ui/interfaces.h>

#include <stdio.h> // Mientras se prueba la aplicación.

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

    w = webview_create(0, NULL); // Crear la ventan
    if (!w)
    {
        puts("No se pudo crear la ventana.");
        return APP_FAILURE;
    }

    // Configuraciones iniciales de la ventana
    webview_set_title(w, "VibeCast");                 // Establecer el título de la ventana
    webview_set_size(w, 480, 320, WEBVIEW_HINT_NONE); // Establecer el tamaño de la ventana

    // Enlazar funciones para controlar la interfaz gráfica
    webview_bind(w, "enviarMensaje", handle_message, *appstate); // Para enlazar la función que maneja mensajes enviados desde JavaScript

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
    puts("Mensaje recibido desde JS.");
    printf("id: %s\n", id);
    printf("req: %s\n", req);

    char response[256];
    snprintf(response, sizeof(response), "{\"status\": \"Recibido: %s\"}", req);
    webview_return(w, id, 0, response);
}
