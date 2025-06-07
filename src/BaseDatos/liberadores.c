#include <BaseDatos/liberadores.h>

void liberar(Reproduccion, void *repr)
{
    if (!repr)
        return;

    free(cast(Reproduccion *, repr)->fecha);
}

void liberar(Playlist, void *playlist)
{
    if (!playlist)
        return;

    Playlist *pl = playlist;

    free(pl->nombre);
    liberar(Lista, &pl->canciones, NULL);
}