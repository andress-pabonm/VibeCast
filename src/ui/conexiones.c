#include <ui/interfaces.h>

/* ======== Para conectar las interfaces ======== */

bool VibeCast_InitBindings()
{
    char *msg = NULL;

    bind_fn(is_logged_in, &msg);
    bind_fn(iniciar_sesion, &msg);
    bind_fn(cerrar_sesion, &msg);
    bind_fn(crear_cuenta, &msg);
    bind_fn(get_user_data, &msg);
    bind_fn(actualizar_usuario, &msg);
    bind_fn(actualizar_password, &msg);

    bind_fn(get_amigos, &msg);

    bind_fn(next_song, &msg);
    bind_fn(get_canciones, &msg);
    bind_fn(get_recomendaciones, &msg);

    // === HISTORIAL ===
    bind_fn(mostrar_historial, NULL);
    bind_fn(vaciar_historial, NULL);

    // === COLA DE REPRODUCCIÓN ===
    bind_fn(mostrar_cola, NULL);
    bind_fn(vaciar_cola, NULL);
    bind_fn(enqueue, &msg);
    bind_fn(dequeue, &msg);

    bind_fn(get_playlists, &msg);

    bind_fn(get_artist_data, &msg);

    return true;
}
