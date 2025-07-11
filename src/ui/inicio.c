#include <ui/interfaces.h>

/* ======== Cargar canciones en el menú principal ======== */

message_handler(get_canciones)
{
    json_object *array = json_object_new_array();

    forEachInLista(canciones, getSongJSON, array);

    VibeCast_SendArray(
        id,
        HTTP_OK,
        array,
        "Canciones cargadas",
        STATE_SUCCESS);

    json_object_put(array);
}

/* ======== Cargar recomendaciones en el menú principal ======== */

new_operfn(rehacerHistorial)
{
    insertValueInPila(arg, val);
    return FOREACH_CONTINUE;
}

new_operfn(getHistorialAmigos)
{
    Usuario *amigo = val;

    Pila historial = amigo->historial.reproducciones;
    Pila tmpHistorial = newPila();

    Reproduccion *repr = deleteValueInPila(historial);
    Cancion *cancion;

    while (repr)
    {
        // Buscar por si ya está añadida
        cancion = searchValueInLista(
            arg,
            &repr->cancion->id,
            cmpCancionConId);

        if (!cancion)
        {
            // Buscar en la lista global
            cancion = searchValueInLista(
                canciones,
                &repr->cancion->id,
                cmpCancionConId);

            insertValueInLista(arg, cancion);
        }

        // Apilar en el historial temporal
        insertValueInPila(tmpHistorial, repr);

        // Desapilar del historial
        repr = deleteValueInPila(historial);
    }

    // Rehacer historial
    destroyPila(tmpHistorial, rehacerHistorial, historial);

    return FOREACH_CONTINUE;
}

Lista cancionesRecomendadas()
{
    // Crear una lista para las canciones a recomendar
    Lista recomendaciones = newLista(NULL); // Crear una nueva lista para las canciones recomendadas

    // Obtener las canciones del historial de cada amigo
    forEachInLista(usuario->amigos, getHistorialAmigos, recomendaciones); // Obtenemos la lista de recomendaciones de amigos

    // Retornar la lista de recomendaciones
    return recomendaciones;
}

new_operfn(getRecomendacionJSON)
{
    Cancion *cancion = val;

    return FOREACH_CONTINUE;
}

message_handler(get_recomendaciones)
{
    json_object *array = json_object_new_array();

    destroyLista(
        cancionesRecomendadas(),
        getRecomendacionJSON,
        array);

    VibeCast_SendArray(id, HTTP_OK, array, "Recomendaciones cargadas", STATE_SUCCESS);
    json_object_put(array);
}