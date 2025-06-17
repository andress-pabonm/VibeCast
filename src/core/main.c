#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

#include <VibeCastConfig.h> // Configuración de VibeCast

#include <sqlite3.h>         // Para la base de datos
#include <webview/webview.h> // Para la interfaz gráfica

#include <stdio.h>

void handle_message(const char *seq, const char *req, void *arg);

AppResult AppInit(void **appstate, int argc, char *argv[])
{
    puts("Aplicación inicializada");

    return APP_CONTINUE; // Continuar con la ejecución
}

AppResult AppIterate(void *appstate)
{
    // Código de ejemplo

    webview_t w = webview_create(0, NULL);            // Crear la ventana
    webview_set_title(w, "VibeCast");                 // Establecer el título de la ventana
    webview_set_size(w, 480, 320, WEBVIEW_HINT_NONE); // Establecer el tamaño de la ventana

    webview_bind(w, "enviarMensaje", handle_message, w); // Para enlazar la función que maneja mensajes enviados desde JavaScript

    // webview_set_html(w, "Thanks for using webview!"); // Para poner un hetml directamente
    webview_navigate(w, "file://" VIBECAST_UI_DIR "/Login/index.html");

    webview_run(w);     // Ejecutar la ventana (bucle principal de la ventana)
    webview_destroy(w); // Destruir la ventana

    return APP_SUCCESS; // Finalizar ejecución
}

void AppQuit(void *appstate, AppResult appresult)
{
    puts("Ejecución finalizada");
}

void handle_message(const char *seq, const char *req, void *arg)
{
    printf("Mensaje recibido desde JS: %s\n", req);
    char response[256];
    snprintf(response, sizeof(response), "{\"status\": \"Recibido: %s\"}", req);
    webview_return((webview_t)arg, seq, 0, response);
}
