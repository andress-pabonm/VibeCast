#include <ui/interfaces.h>

/* ================================================================ */
// Funciones auxiliares
/* ================================================================ */

static Lista cancionesRecomendadas();
static new_operfn(obtenerHistorialAmigos);
static new_operfn(rehacerHistorial);

/* ================================================================ */
// Cargar canciones en la sección de Inicio
/* ================================================================ */

message_handler(obtener_canciones)
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

/* ================================================================ */
// Cargar reomendaciones en la sección de Inicio
/* ================================================================ */

message_handler(obtener_recomendaciones)
{
    json_object *array = json_object_new_array();

    destroyLista(
        cancionesRecomendadas(),
        getSongJSON,
        array);

    VibeCast_SendArray(
        id,
        HTTP_OK,
        array,
        "Recomendaciones cargadas",
        STATE_SUCCESS);

    json_object_put(array);
}

static Lista cancionesRecomendadas()
{
    // Crear una lista para las canciones a recomendar
    Lista recomendaciones = newLista(NULL);

    // Obtener las canciones del historial de cada amigo
    forEachInLista(usuario->amigos,
                   obtenerHistorialAmigos,
                   recomendaciones);

    // Retornar la lista de recomendaciones
    return recomendaciones;
}

static new_operfn(obtenerHistorialAmigos)
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

static new_operfn(rehacerHistorial)
{
    insertValueInPila(arg, val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// Generar reporte de estadísticas
/* ================================================================ */

message_handler(generar_reporte)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}
