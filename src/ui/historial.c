#include <ui/interfaces.h>
#include <utils/utils.h>

void agregarAHistorial(Usuario *usuario, Cancion *cancion, Anuncio *anuncio) {

    if (!usuario || !cancion || !anuncio) return;

    //Ve si llega un anuncio antes que una canción
    //Sube el contador de anuncios si el usuario es freemium
    if (anuncio && usuario->plan == PLAN_FREEMIUM) {
        // Increment the number of ads viewed if the user is on a freemium plan
        usuario->historial.cantidadAnuncios++;
        return;
    }

    Reproduccion *reproduccion = newReproduccion();
    if (!reproduccion) return;

    reproduccion->cancion = cancion;
    reproduccion->fechaEscuchado = asprintf("Fecha y hora actual"); // Placeholder for actual date/time

    // Insert the reproduction into the user's history
    insertValueInPila(usuario->historial.reproducciones, reproduccion);
    
    // Update the total time listened and number of ads
    usuario->historial.tiempoEscuchado += cancion->duracion;
}

void mostrarHistorial(Usuario *usuario) {

    if (!usuario || !usuario->historial.reproducciones) return;

    Pila *actual = usuario->historial.reproducciones->top;
    while (actual) {
        Reproduccion *reproduccion = actual->data;
        if (!reproduccion || !reproduccion->cancion) continue;
        printf("Canción: %s, Fecha: %s\n", reproduccion->cancion->nombre, reproduccion->fechaEscuchado);
        actual = actual->next;
    }
}

void vaciarHistorial(Usuario *usuario) {
    
    if (!usuario || !usuario->historial.reproducciones) return;

    destroyPila(usuario->historial.reproducciones, NULL, NULL);

    usuario->historial.reproducciones = newPila();
    usuario->historial.tiempoEscuchado = 0;
    usuario->historial.cantidadAnuncios = 0;
}