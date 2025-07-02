#include <ui/interfaces.h>
#include <utils/utils.h>

new_cmpfn(cmpArtistaConNombre){
    const Artista *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

Artista *crearArtista(const char *nombre)
{
    if(searchValueInABB(artistas, nombre, cmpArtistaConNombre))
    {
        printf("Error: El artista '%s' ya existe\n", nombre);

        return NULL; // No se puede crear un artista con un nombre ya existente
    }


    Artista *artista = newArtista(); //Todos sus datos inician en null
    if (!artista) return;

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