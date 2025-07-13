#include <ui/conexiones.h>

/* ======== Para conectar las interfaces ======== */

bool VibeCast_InitBindings()
{
    char *msg = NULL;

    /* ==== Cola de reproducción ==== */

    bind_fn(encolar, &msg);     //  Agregar canción a cola de reproducción
    bind_fn(decolar, &msg);     // Obtener siguiente canción o anuncio en  cola de reproducción
    bind_fn(vaciar_cola, &msg); // Vaciar cola de reproducción

    /* ==== Inicio ==== */

    bind_fn(obtener_canciones, &msg);       // Para obtener las canciones a mostrar en la sección Inicio
    bind_fn(obtener_recomendaciones, &msg); // Para obtener las recomendaciones a mostrar en la sección Inicio
    bind_fn(generar_reporte, &msg);         // Para generar un reporte con las estadísticas de la aplicación

    /* ==== Biblioteca ==== */

    bind_fn(obtener_playlists, &msg);   // Para obtener las playlists del usuario activo
    bind_fn(crear_playlist, &msg);      // Crear playlist
    bind_fn(eliminar_playlist, &msg);   // Eliminar playlist
    bind_fn(actualizar_playlist, &msg); // Actualizar información de playlist

    bind_fn(agregar_a_playlist, &msg);   // Agregar canción a playlist
    bind_fn(eliminar_de_playlist, &msg); // Quitar canción de playlist

    /* ==== Historial ==== */

    bind_fn(obtener_historial, &msg); // Para obtener el historial del usuario activo
    bind_fn(vaciar_historial, &msg);

    /* ==== Amigos ==== */

    bind_fn(obtener_amigos, &msg); // Para obtener la lista de amigos del usuario activo
    bind_fn(agregar_amigo, &msg);  // Para agregar un amigo
    bind_fn(eliminar_amigo, &msg); // Para eliminar un amigo

    /* ==== Perfil ==== */

    // Usuario

    bind_fn(sesion_activa, &msg);  // Verificar si ha iniciado sesión
    bind_fn(iniciar_sesion, &msg); // Iniciar sesión
    bind_fn(cerrar_sesion, &msg);  // Cerrar sesión

    bind_fn(crear_cuenta, &msg);    // Crear cuenta
    bind_fn(eliminar_cuenta, &msg); // Eliminar cuenta

    bind_fn(obtener_info_usuario, &msg);    // Para obtener la información del usuario activo
    bind_fn(actualizar_info_usuario, &msg); // Para actualizar los datos del usuario
    bind_fn(actualizar_password, &msg);     // Para actualizar la contraseña
    bind_fn(activar_premium, &msg);         // Para activar el plan premium

    // Artista

    bind_fn(obtener_info_artista, &msg); // Para obtener la información de artista del usuario activo
    bind_fn(crear_artista, &msg);        // Crear perfil de artista
    bind_fn(eliminar_artista, &msg);     // Eliminar perfil de artista

    bind_fn(crear_album, &msg);      // Crear álbum
    bind_fn(eliminar_album, &msg);   // Eliminar álbum
    bind_fn(actualizar_album, &msg); // Actualizar información de álbum

    bind_fn(crear_cancion, &msg);      // Crear canción
    bind_fn(eliminar_cancion, &msg);   // Eliminar canción
    bind_fn(actualizar_cancion, &msg); // Actualizar información de canción

    return true;
}
