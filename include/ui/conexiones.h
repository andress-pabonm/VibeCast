#ifndef VIBECAST_CONEXIONES_H
#define VIBECAST_CONEXIONES_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>
#include <json.h>

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

// Para conectar una función con la interfaz gráfica
bool VibeCast_BindFn(const char *name, message_handler_t msgh, void *arg);
// Macro para facilitar usar BindFN
#define bind_fn(fn, arg) func(BindFn, stringify(fn), fn, arg)

/* ================================================================ */

// Envía un valor booleano (true/false)
bool VibeCast_SendBool(const char *id, int status_code, bool value, const char *msg, const char *status);

// Envía un número entero
bool VibeCast_SendInt(const char *id, int status_code, int value, const char *msg, const char *status);

// Envía un número decimal (double)
bool VibeCast_SendFloat(const char *id, int status_code, double value, const char *msg, const char *status);

// Envía una cadena de texto
bool VibeCast_SendText(const char *id, int status_code, const char *text, const char *msg, const char *status);

// Envía un array JSON
bool VibeCast_SendArray(const char *id, int status_code, json_object *array, const char *msg, const char *status);

// Envía un objeto JSON
bool VibeCast_SendJSON(const char *id, int status_code, json_object *object, const char *msg, const char *status);

// Envía un valor nulo explícito
bool VibeCast_SendNull(const char *id, int status_code, const char *msg, const char *status);

// Envía cualquier objeto JSON (core)
bool VibeCast_SendObj(const char *id, int status_code, const char *type, json_object *data, const char *msg, const char *status);

// Estados generales
#define STATUS_OK "ok"
#define STATUS_ERROR "error"

// Estado lógico como string
#define STATE_SUCCESS STATUS_OK    // operación exitosa
#define STATE_FAILURE STATUS_ERROR // operación fallida

#define STATE_BOOL(cond) ((cond) ? STATE_SUCCESS : STATE_FAILURE)
#define STATUS_BOOL(cond) ((cond) ? STATUS_OK : STATUS_ERROR)

// Códigos exitosos
#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED 202
#define HTTP_NO_CONTENT 204

// Códigos de error del cliente
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_CONFLICT 409

// Códigos de error del servidor
#define HTTP_INTERNAL_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501

/* ================================================================ */

// Funciones de conexión

/* ================================================================ */

bool VibeCast_InitBindings();

#endif // VIBECAST_CONEXIONES_H