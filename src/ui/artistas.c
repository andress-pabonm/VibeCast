#include <ui/interfaces.h>
#include <utils/utils.h>

Artista *crearArtista(const char *nombre)
{
    Artista *artista = newArtista();
    if (!artista)
        return;
    usuario->artista = artista;
    artista->nombre = asprintf(nombre);
    artista->usuario = usuario;
    return artista;
}

void insertarArtistaEnABB(Artista *artista)
{
    insertValueInABB(artistas, artista);
}

void agregarAlbum(Album *album)
{
    insertValueInLista(usuario->artista->albumes, album);
}
