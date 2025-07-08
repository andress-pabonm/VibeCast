#include <ui/interfaces.h>
#include <utils/utils.h>

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