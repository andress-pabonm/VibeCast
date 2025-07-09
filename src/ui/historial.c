#include <ui/interfaces.h>

bool agregarAHistorial(Usuario *usuario, Cancion *cancion, Anuncio *anuncio)
{

    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());

    // Ve si llega un anuncio antes que una canción
    // Sube el contador de anuncios si el usuario es freemium
    if (anuncio && usuario->plan == PLAN_FREEMIUM)
    {
        // Increment the number of ads viewed if the user is on a freemium plan
        usuario->historial.cantidadAnuncios++;
        return true;
    }

    Reproduccion *reproduccion = newReproduccion();

    reproduccion->cancion = cancion;
    reproduccion->fechaEscuchado = asprintf(fechaActual);

    // Inserta la reproducción en el historial
    insertValueInPila(usuario->historial.reproducciones, reproduccion);

    // Actualiza el tiempo escuchado
    usuario->historial.tiempoEscuchado += cancion->duracion;

    return true;
}

bool mostrarHistorial(Usuario *usuario)
{
    // Pila *actual = usuario->historial.reproducciones->top;
    Pila historial = usuario->historial.reproducciones;
    Reproduccion *repr = NULL;

    repr = deleteValueInPila(historial);

    while (repr != NULL)
    {
        if (repr)
        {
            printf("Canción: %s, Fecha: %s\n", repr->cancion->nombre, repr->fechaEscuchado);
        }

        repr = deleteValueInPila(historial);
    }

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

//===============================================================
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

message_handler(mostrar_historial)
{
    VibeCast_MostrarHistorial(NULL, 0, NULL, NULL);
    VibeCast_SendText(id, HTTP_OK, "Historial mostrado en consola", "Historial", STATE_SUCCESS);
}

//======================================================================

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

//======================================================================
interfaz(VaciarHistorial)
{
    destroyPila(usuario->historial.reproducciones, destroyReproducciones, NULL);
    usuario->historial.reproducciones = newPila();
    usuario->historial.tiempoEscuchado = 0;
    usuario->historial.cantidadAnuncios = 0;

    send_message("Historial vaciado correctamente");

    return true;
}

message_handler(vaciar_historial)
{
    VibeCast_VaciarHistorial(NULL, 0, NULL, NULL);
    VibeCast_SendText(id, HTTP_OK, "", "Historial vaciado", STATE_SUCCESS);
}
