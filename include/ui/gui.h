#ifndef VIBECAST_GUI_H
#define VIBECAST_GUI_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>

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

/* Funciones principales */

bool VibeCast_InitGUI();    // Inicializar la interfaz gráfica
bool VibeCast_RunGUI();     // Para ejecutar la interfaz gráfica
bool VibeCast_DestroyGUI(); // Liberar la memoria de la interfaz gráfica

// Para conectar una función con la interfaz gráfica
bool VibeCast_BindFn(const char *name, message_handler_t msgh, void *arg);
// Macro para facilitar usar BindFN
#define bind_fn(fn, arg) func(BindFn, stringify(fn), fn, arg)

// Para enviar un mensaje a la interfaz gráfica
bool VibeCast_SendMsg(const char *id, int status, const char *msg);
// Macro para facilitar el uso de sendMessage
#define send_message(status, message) VibeCast_SendMsg(id, status, message)

#endif // VIBECAST_GUI_H