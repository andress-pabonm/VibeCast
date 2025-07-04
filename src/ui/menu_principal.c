#include <ui/interfaces.h>

json_object *cancion_to_json(Cancion *cancion)
{
    json_object *c = json_object_new_object();

    // title
    json_object_object_add(c, "title", json_object_new_string(cancion->nombre));

    // artist
    json_object_object_add(c, "artist", json_object_new_string(cancion->album->artista->nombre));

    // duration
    json_object_object_add(c, "duration", json_object_new_int(cancion->duracion));

    return c;
}

new_operfn(getCanciones)
{
    json_object_array_add(arg, cancion_to_json(val));
    return FOREACH_CONTINUE;
}

message_handler(get_canciones)
{
    json_object *array = json_object_new_array();

    forEachInLista(canciones, getCanciones, array);

    size_t len = json_object_array_length(array);

    VibeCast_SendArray(
        id,
        (len > 0) ? HTTP_OK : HTTP_NO_CONTENT,
        array,
        "Canciones"
        "Lista de canciones",
        STATE_SUCCESS);
}

/* ==== */