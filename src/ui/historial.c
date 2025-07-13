#include <ui/interfaces.h>

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

bool agregarAHistorial(Usuario *usuario, Cancion *cancion, Anuncio *anuncio);
bool vaciarHistorial(Usuario *usuario);
static interfaz(ObtenerHistorial);

/* ================================================================ */
// BLOQUE: obtener_historial — Mostrar historial (actualmente inactivo)
/* ================================================================ */

message_handler(obtener_historial)
{
    json_object *array = json_object_new_array();

    char **msg = arg;

    bool success = VibeCast_ObtenerHistorial(array, 0, NULL, msg);
    VibeCast_SendArray(id, HTTP_OK, array, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

static interfaz(ObtenerHistorial)
{
    json_object *jobj = json_object_new_object();

    Pila historial = usuario->historial.reproducciones;
    Pila tmpHistorial = newPila();

    Reproduccion *repr = deleteValueInPila(historial);

    while (repr)
    {
        json_object_object_add(jobj, "songId", json_object_new_int(repr->cancion->id));
        json_object_object_add(jobj, "title", json_object_new_string(repr->cancion->nombre));
        json_object_object_add(jobj, "artist", json_object_new_string(repr->cancion->album->artista->nombre));
        json_object_object_add(jobj, "duration", json_object_new_int(repr->cancion->duracion));
        json_object_object_add(jobj, "playedAt", json_object_new_string(repr->fechaEscuchado));

        json_object_array_add(arg, jobj);

        insertValueInPila(tmpHistorial, repr);

        repr = deleteValueInPila(historial);
    }

    destroyPila(tmpHistorial, rehacerHistorial, historial);

    return true;
}

/* ================================================================ */
// BLOQUE: agregar_a_historial — Agregar una canción al historial
/* ================================================================ */

message_handler(agregar_a_historial)
{
    init_data_json();

    const char *id_cancion = get_string(get_array_idx(data, 0));

    const char *argv[] = {id_cancion};
    char **msg = arg;
    bool ok = VibeCast_AgregarAHistorial(NULL, 1, argv, msg);

    VibeCast_SendText(id, ok ? HTTP_OK : HTTP_BAD_REQUEST, *msg, "Agregar a historial", STATE_BOOL(ok));
    freem(*msg);
    *msg = NULL;
}

interfaz(AgregarAHistorial)
{
    int id = atoi(argv[0]);

    Cancion *c = searchValueInLista(canciones, &id, cmpCancionConId);
    if (!c)
    {
        send_message("No se encontró la canción con ID %d", id);
        return false;
    }

    agregarAHistorial(usuario, c, NULL);
    send_message("Canción '%s' agregada al historial.", c->nombre);

    return true;
}

bool agregarAHistorial(Usuario *usuario, Cancion *cancion, Anuncio *anuncio)
{
    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());

    if (anuncio && usuario->plan == PLAN_FREEMIUM)
    {
        usuario->historial.cantidadAnuncios++;
        return true;
    }

    Reproduccion *reproduccion = newReproduccion();
    reproduccion->cancion = cancion;
    reproduccion->fechaEscuchado = asprintf(fechaActual);

    insertValueInPila(usuario->historial.reproducciones, reproduccion);
    usuario->historial.tiempoEscuchado += cancion->duracion;

    printf("Me gusta el sexo lesbico\nAñanadiendo cancion %s\n", cancion->nombre);

    return true;
}

/* ================================================================ */
// BLOQUE: vaciar_historial — Vaciar historial del usuario
/* ================================================================ */

message_handler(vaciar_historial)
{
    VibeCast_VaciarHistorial(NULL, 0, NULL, NULL);
    VibeCast_SendNull(id, HTTP_OK, "Historial vaciado", STATE_SUCCESS);
}

interfaz(VaciarHistorial)
{
    destroyPila(usuario->historial.reproducciones, destroyReproducciones, NULL);
    usuario->historial.reproducciones = newPila();
    usuario->historial.tiempoEscuchado = 0;
    usuario->historial.cantidadAnuncios = 0;

    send_message("Historial vaciado correctamente");

    return true;
}

bool vaciarHistorial(Usuario *usuario)
{
    destroyPila(usuario->historial.reproducciones, NULL, NULL);

    usuario->historial.reproducciones = newPila();
    usuario->historial.tiempoEscuchado = 0;
    usuario->historial.cantidadAnuncios = 0;

    return true;
}

/* ================================================================ */
// BLOQUE: mostrar_historial (interfaz) — Mostrar historial en consola
/* ================================================================ */

interfaz(MostrarHistorial)
{
    Pila pila = usuario->historial.reproducciones;
    Pila temp = newPila();

    Reproduccion *r = deleteValueInPila(pila);

    if (!r)
    {
        send_message("El historial está vacío.");
        return true;
    }

    printf("Historial de %s:\n", usuario->username);

    while (r)
    {
        if (r->cancion)
            printf("- %s | escuchado en: %s\n", r->cancion->nombre, r->fechaEscuchado);

        insertValueInPila(temp, r);
        r = deleteValueInPila(pila);
    }

    while ((r = deleteValueInPila(temp)) != NULL)
        insertValueInPila(pila, r);

    destroyPila(temp, NULL, NULL);

    return true;
}
