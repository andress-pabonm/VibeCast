#include <ui/interfaces.h>

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

bool agregarAHistorial(Usuario *usuario, Cancion *cancion, Anuncio *anuncio);
bool mostrarHistorial(Usuario *usuario);
bool vaciarHistorial(Usuario *usuario);
Lista getHistorial();

/* ================================================================ */
// BLOQUE: mostrar_historial — Mostrar historial (actualmente inactivo)
/* ================================================================ */

message_handler(mostrar_historial)
{
    // VibeCast_MostrarHistorial(NULL, 0, NULL, NULL);
    // VibeCast_SendText(id, HTTP_OK, "Historial mostrado en consola", "Historial", STATE_SUCCESS);
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

    return true;
}

/* ================================================================ */
// BLOQUE: obtener_historial — Obtener historial (no implementado)
/* ================================================================ */

message_handler(obtener_historial)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

Lista getHistorial()
{
    Pila historial = usuario->historial.reproducciones;
    Pila tmpHistorial = newPila();
    Lista reprs = newLista(NULL);

    return reprs;
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

bool mostrarHistorial(Usuario *usuario)
{
    Pila historial = usuario->historial.reproducciones;
    Reproduccion *repr = deleteValueInPila(historial);

    while (repr != NULL)
    {
        if (repr)
            printf("Canción: %s, Fecha: %s\n", repr->cancion->nombre, repr->fechaEscuchado);

        repr = deleteValueInPila(historial);
    }

    return true;
}
