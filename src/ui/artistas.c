#include <ui/interfaces.h>
#include <utils/utils.h>

new_cmpfn(cmpArtistaConNombre){
    const Artista *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

new_cmpfn(cmpAlbumConNombre){
    const Album *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

bool *crearArtista(const char *nombreArtista)
{
    //Buscamos si el nombre del artista ya existe
    if(searchValueInABB(artistas, nombreArtista, cmpArtistaConNombre))
    {
        printf("Error: El artista '%s' ya existe\n", nombreArtista);

        return NULL; // No se puede crear un artista con un nombre ya existente
    }

    //CREAMOS ARTISTA
    Artista *artista = newArtista(); //Todos sus datos inician en null
    if (!artista) return;

    usuario->artista = artista; //Agregamos la estructura artista al usuario
    artista->nombre = asprintf(nombreArtista);
    artista->usuario = usuario;

    //Insertamos en el arbol
    insertValueInABB(artistas, artista);
}

bool crearAlbum(const char *nombreAlbum){
    //Buscamos si el album ya existe en los albumes del artista
    if(searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre)){
        printf("Error: El álbum '%s' ya existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return NULL; // No se puede crear un álbum con un nombre ya existente
    }

    //CREAMOS NUEVO ALBUM
    Album *album = newAlbum(); // Todos sus datos inician en null

    usuario->artista->albumes = album; //Agregamos la estructura album al artista
    album->nombre = asprintf(nombreAlbum);  
    album->artista = usuario->artista; //AGREGAMOS ALBUM A ARTISTA

    //Insertamos en la lista
    insertValueInLista(usuario->artista->albumes, album);
}

bool agregarCancionAlbum(const char *nombreAlbum, const char *nombreCancion){
    //Buscamos el album
    if (!searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre)) {
        printf("Error: El álbum '%s' no existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return NULL; // No se puede agregar una canción a un álbum que no existe
    }

    //Creamos nueva canción(No se si la logica seria aqui)
    Cancion *cancion = newCancion(); // Todos sus datos inician en null

    usuario->artista->albumes = cancion->album; //Agregamos la estructura cancion al album (creo q esta bien)
    cancion->nombre = asprintf(nombreCancion);
    cancion->album = usuario->artista->albumes; //AGREGAMOS CANCION A ALBUM

    //Insertamos en la lista
    insertValueInLista(cancion->album, cancion);
}