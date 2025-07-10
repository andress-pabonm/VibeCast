#include <ui/interfaces.h>

message_handler(get_canciones)
{
    json_object *array = json_object_new_array();

    forEachInLista(canciones, getSongJSON, array);

    size_t len = json_object_array_length(array);

    VibeCast_SendArray(
        id,
        (len > 0) ? HTTP_OK : HTTP_NO_CONTENT,
        array,
        "Canciones"
        "Lista de canciones",
        STATE_SUCCESS);

    json_object_put(array);
}

/* ================================================================ */

message_handler(get_recomendaciones)
{
    Lista recomendaciones = recomendarCanciones();

    json_object *array = json_object_new_array();
    VibeCast_SendArray(id, HTTP_OK, array, "Recomendaciones cargadas", STATE_SUCCESS);
    json_object_put(array);

    destroyLista(recomendaciones, NULL, NULL);
}