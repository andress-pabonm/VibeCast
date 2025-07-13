#ifndef VIBECAST_CONEXIONES_H
#define VIBECAST_CONEXIONES_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>
#include <utils/utils.h>
#include <json.h>

/* ======== Comunicación con webview ======== */

/**
 * Tipo para funciones de comunicación con webview.
 * @param id: Identificador de la solicitud.
 * @param req: Petición enviada por la interfa1z gráfica. Formato: "[contenido_de_la_solicitud]"
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

// Envía cualquier objeto JSON (core)
bool VibeCast_SendObj(const char *id, int status_code, const char *type, json_object *data, const char *msg, const char *status);

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

// Envía un valor nulo explícito junto a un mensaje de error
bool VibeCast_SendError(const char *id, int status_code, const char *msg, const char *status);

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

/* ================ Utilidades ================ */

// JSON-C helpers
#define new_json_object json_object_new_object
#define new_json_array json_object_new_array
#define get_json json_tokener_parse
#define get_array_idx json_object_array_get_idx

static inline json_object *get_field(json_object *src, const char *field)
{
    json_object *dest;
    json_object_object_get_ex(src, field, &dest);
    return dest;
}

#define init_json(dest, src) json_object *dest = src
#define end_json json_object_put
#define init_data_json() init_json(data, get_json(req))
#define end_data_json() end_json(data);
#define get_string json_object_get_string
#define get_int json_object_get_int

/* ================================================================ */

// ================================
// Funciones de conexión
// ================================

/* ==== Sesión ==== */

message_handler(sesion_activa);  // Verificar si ha iniciado sesión
message_handler(iniciar_sesion); // Iniciar sesión
message_handler(cerrar_sesion);  // Cerrar sesión

/* ==== Cola de reproducción ==== */

message_handler(encolar);     //  Agregar canción a cola de reproducción
message_handler(decolar);     // Obtener siguiente canción o anuncio en  cola de reproducción
message_handler(vaciar_cola); // Vaciar cola de reproducción

/* ==== Inicio ==== */

message_handler(obtener_canciones);       // Para obtener las canciones a mostrar en la sección Inicio
message_handler(obtener_recomendaciones); // Para obtener las recomendaciones a mostrar en la sección Inicio
message_handler(generar_reporte);         // Para generar un reporte con las estadísticas de la aplicación

/* ==== Biblioteca ==== */

message_handler(obtener_playlists);   // Para obtener las playlists del usuario activo
message_handler(crear_playlist);      // Crear playlist
message_handler(eliminar_playlist);   // Eliminar playlist
message_handler(actualizar_playlist); // Actualizar información de playlist

message_handler(agregar_a_playlist);   // Agregar canción a playlist
message_handler(eliminar_de_playlist); // Eliminar canción de playlist

/* ==== Historial ==== */

message_handler(obtener_historial); // Para obtener el historial del usuario activo
message_handler(vaciar_historial);

/* ==== Amigos ==== */

message_handler(obtener_amigos); // Para obtener la lista de amigos del usuario activo
message_handler(agregar_amigo);  // Para agregar un amigo
message_handler(eliminar_amigo); // Para eliminar un amigo

/* ==== Perfil ==== */

// Usuario

message_handler(crear_cuenta);    // Crear cuenta
message_handler(eliminar_cuenta); // Eliminar cuenta

message_handler(obtener_info_usuario);    // Para obtener la información del usuario activo
message_handler(actualizar_info_usuario); // Para actualizar los datos del usuario
message_handler(actualizar_password);     // Para actualizar la contraseña
message_handler(activar_premium);         // Para activar el plan premium

// Artista

message_handler(obtener_info_artista); // Para obtener la información de artista del usuario activo
message_handler(crear_artista);        // Crear perfil de artista
message_handler(eliminar_artista);     // Eliminar perfil de artista

message_handler(crear_album);      // Crear álbum
message_handler(eliminar_album);   // Eliminar álbum
message_handler(actualizar_album); // Actualizar información de álbum

message_handler(crear_cancion);      // Crear canción
message_handler(eliminar_cancion);   // Eliminar canción
message_handler(actualizar_cancion); // Actualizar información de canción

/* ================================================================ */

bool VibeCast_InitBindings();

#endif // VIBECAST_CONEXIONES_H