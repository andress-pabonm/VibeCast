#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

#include <VibeCastConfig.h>
#include <db/datos.h>
#include <utils/utils.h>

#include <webview/webview.h> // Para la interfaz gráfica

/* +--------------------------+ */
/* |        Utilidades        | */
/* +--------------------------+ */

// Estructura para representar el estado de la aplicación
typedef struct
{
    // Ventana actual
    webview_t w;
} AppState;

// Para enlazar funciones para las interfaces
#define bind_fn(window, fn, arg) webview_bind(window, stringify(fn), fn, arg)

// Para responder a la interfaz gráfica
#define send_message(status_code, message) webview_return(cast(AppState *, arg)->w, id, status_code, stringify({"message" : message}))

#define TRUE 1
#define FALSE 0
#define STATUS_SUCCESS TRUE
#define STATUS_FAILURE FALSE

/**
 * Macro para facilitar la llamada a los archivos .html
 * Utilizas la ruta relativa al directorio ui donde están las .html
 * Uso: INTERFAZ("carpeta/archivo.html")
 */
#define INTERFAZ(nombre) "file://" path_to("ui/" nombre)

/* ======== Comunicación con webview ======== */

/**
 * Tipo para funciones de comunicación con webview.
 * @param id: Identificador de la solicitud.
 * @param req: Petición enviada por la interfaz gráfica. Formato: "[contenido_de_la_solicitud]"
 * @param arg: Argumento pasado al enlazar la función. (Será el estado de la aplicación)
 */
typedef void (*message_handler_t)(const char *id, const char *req, void *arg);

/**
 * Macro para declarar y definir fácilmente funciones de tipo (message_handler_t)
 * @param name: Nombre de la función
 */
#define message_handler(name) void name(const char *id, const char *req, void *arg)

/* ==== Funciones para manejar las interfaces ======== */

message_handler(test);
message_handler(is_logged_in);
message_handler(iniciar_sesion);
message_handler(crear_cuenta);
message_handler(cerrar_sesion);

#endif // VIBECAST_INTERFACES_H