#ifndef VIBECAST_CONEXIONES_H
#define VIBECAST_CONEXIONES_H

#include <VibeCastConfig.h>
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

message_handler(test); // Conexión para comprobar los datos enviados desde JS

// Conxiones con las interfaces de sesión de usuario

message_handler(is_logged_in);   // Preguntar si hay una sesión abierta
message_handler(iniciar_sesion); // Conexión para el inicio de sesión
message_handler(crear_cuenta);   // Conexión para la creación de cuenta
message_handler(cerrar_sesion);  // Conexión para cerrar sesión

message_handler(get_userdata); // Para solicitar la información de un usuario

// Funciones de conexión con el artista

message_handler(get_artist_data); // Para solicitar la información de un artista
message_handler(get_albumes);     // Para solicitar los álbumes de un artista
message_handler(get_canciones);   // Para solicitar las canciones de un álbum

message_handler(crear_artista);    // Para crear un perfil de artista
message_handler(crear_album);      // Para crear un álbum
message_handler(publicar_cancion); // Para crear/publicar una canción dentro de un álbum

// Funciones de conexión para crear anuncios

message_handler(crear_anuncio); // Para crear un nuevo anuncio

// Funciones de conexión para la cola de reproducción

message_handler(add_song_to_queue);     // Añadir canción a la cola
message_handler(add_playlist_to_queue); // Añadir playlist a la cola
message_handler(next_song);             // Solicitar siguiente canción en la cola
message_handler(empty_queue);           // Vaciar cola

#endif // VIBECAST_CONEXIONES_H