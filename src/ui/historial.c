#include <ui/interfaces.h>

/* ================================================================ */
// DEFINICIÓN DE ESTRUCTURAS INTERNAS
/* ================================================================ */

typedef struct
{
    Pila tmpHistorial;
    json_object *reprs_array;
} getReprJSON_arg_t;

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(ObtenerHistorial);
static new_operfn(getReprJSON);

static interfaz(VaciarHistorial);

/* ================================================================ */
// BLOQUE: obtener_historial — Obtener historial del usuario
/* ================================================================ */

message_handler(obtener_historial)
{
    json_object *reprs_array = json_object_new_array();

    char **msg = arg;

    bool success = VibeCast_ObtenerHistorial(reprs_array, 0, NULL, msg);
    VibeCast_SendArray(id, HTTP_OK, reprs_array, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

static interfaz(ObtenerHistorial)
{
    getReprJSON_arg_t wrapper_arg = {
        .tmpHistorial = newPila(),
        .reprs_array = arg,
    };

    destroyPila(usuario->historial.reproducciones, getReprJSON, &wrapper_arg);
    usuario->historial.reproducciones = newPila();
    destroyPila(wrapper_arg.tmpHistorial, rehacerHistorial, usuario->historial.reproducciones);

    return true;
}

static new_operfn(getReprJSON)
{
    getReprJSON_arg_t *wrapper_arg = arg;
    Reproduccion *repr = val;

    insertValueInPila(wrapper_arg->tmpHistorial, repr);

    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "songId", json_object_new_int(repr->cancion->id));
    json_object_object_add(jobj, "title", json_object_new_string(repr->cancion->nombre));
    json_object_object_add(jobj, "artist", json_object_new_string(repr->cancion->album->artista->nombre));
    json_object_object_add(jobj, "duration", json_object_new_int(repr->cancion->duracion));
    json_object_object_add(jobj, "playedAt", json_object_new_string(repr->fechaEscuchado));

    json_object_array_add(wrapper_arg->reprs_array, jobj);

    return FOREACH_CONTINUE;
}

/* ================================================================ */
// BLOQUE: vaciar_historial — Vaciar historial del usuario
/* ================================================================ */

message_handler(vaciar_historial)
{
    VibeCast_VaciarHistorial(NULL, 0, NULL, NULL);
    VibeCast_SendNull(id, HTTP_OK, "Historial vaciado", STATE_SUCCESS);
}

static interfaz(VaciarHistorial)
{
    char *condition = asprintf("id_usuario = %d", usuario->id);
    bool ok = eliminar_registros("Reproducciones", condition, NULL);
    freem(condition);

    if (!ok)
    {
        send_message("No fue posible vaciar el historial.");
        return false;
    }

    destroyPila(usuario->historial.reproducciones, destroyReproducciones, NULL);
    usuario->historial.reproducciones = newPila();

    condition = asprintf("id = %d", usuario->id);
    ok = actualizar_registros("Usuarios", "tiempo_escuchado = 0, cantidad_anuncios = 0", condition, NULL);
    freem(condition);

    if (ok)
    {
        usuario->historial.tiempoEscuchado = 0;
        usuario->historial.cantidadAnuncios = 0;
    }

    return true;
}
