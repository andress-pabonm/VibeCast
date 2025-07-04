#include <ui/interfaces.h>
#include <time.h>

/* ================================ Interfaces ================================ */

static message_handler(crear_artista)
{
    // A implementar
}

message_handler(get_canciones)
{
    puts("Obteniendo canciones");
    VibeCast_SendArray(id, HTTP_OK, NULL, "Canciones cargadas", STATE_SUCCESS);
}

message_handler(get_recomendaciones)
{
    puts("Obteniendo recomendaciones");
    VibeCast_SendArray(id, HTTP_OK, NULL, "Recomendaciones cargadas", STATE_SUCCESS);
}

/* ======== Para conectar las interfaces ======== */

bool VibeCast_InitBindings()
{
    char *msg = NULL;

    bind_fn(is_logged_in, &msg);
    bind_fn(iniciar_sesion, &msg);
    bind_fn(cerrar_sesion, &msg);
    bind_fn(crear_cuenta, &msg);
    bind_fn(get_user_data, &msg);

    bind_fn(next_song, &msg);
    bind_fn(get_canciones, &msg);
    bind_fn(get_recomendaciones, &msg);

    return true;
}
