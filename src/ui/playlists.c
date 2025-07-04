#include <ui/interfaces.h>
#include <utils/utils.h>

static new_cmpfn(cmpPlaylistConNombre)
{
    const Playlist *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

bool crearPlaylist(const char *nombre)
{
    Playlist *nuevaPlaylist = newPlaylist(); // Inicaliza como null

    if (!nuevaPlaylist)
        return false;

    nuevaPlaylist->nombre = asprintf(nombre);

    insertValueInLista(usuario->playlists, nuevaPlaylist); // Insertamos las playlist en lista

    return true;
}

bool agregarCancionPlaylist(const char *nombrePlaylist, int IdCancion)
{
    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre);

    if (!playlist)
    {
        prinf("Error: La playlist '%s' no existe\n", nombrePlaylist);

        return false;
    }

    // BUSCAREMOS EL IDE DE LA CANCION EN LA LISTA GLOBLA DE CANCIONES

    Cancion *cancion = searchValueInLista(canciones, &IdCancion, cmpCancionConId);
    // verificamos si la cancion existe
    if (!cancion)
    {
        printf("Error: La canción con ID %d no existe\n", IdCancion);
        return false;
    }
    // Agrega la canción a la lista de canciones de la playlist
    insertValueInLista(playlist->canciones, cancion);
    return true;
}

bool eliminarCancionPlaylist(int Idcancion, Playlist *playlist)
{

    Cancion *cancionEliminada = deleteValueInLista(playlist->canciones, &Idcancion, cmpCancionConId);

    if (cancionEliminada == NULL)
    {
        printf("Error: La canción %s no se encuentra en la playlist\n", cancionEliminada->nombre);
        return false;
    }
    else
    {
        printf("Canción %s eliminada de la playlist '%s'\n", cancionEliminada->nombre, playlist->nombre);
        return true;
    }
}

bool eliminarPlaylist(const char *nombrePlaylist)
{
    // Buscamos la playlist por nombre
    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre);

    if (!playlist)
    {
        printf("Error: La playlist '%s' no existe\n", nombrePlaylist);
        return false;
    }
    else
    {
        Playlist *playlisteliminada = deleteValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre); // Eliminamos la playlist de lalista de playlists
        if (playlisteliminada == NULL)
        {
            printf("Error: No se pudo eliminar la playlist '%s'\n", nombrePlaylist);
            return false;
        }else{
            printf("Playlist '%s' eliminada correctamente\n", nombrePlaylist);
            // Liberamos la memoria asociada a la playlist
            destroyLista(playlisteliminada->canciones, NULL, NULL); // Liberamos la lista de canciones
            return true;
        }
    }

    // Eliminamos la playlist de la lista de playlists del usuario
    // y liberamos la memoria asociada a la playlist
    if (!usuario || !playlist || !usuario->playlists)
        return false;

    void *eliminado = deleteValueInLista(usuario->playlists, playlist, NULL);
    return eliminado != NULL;
}

void mostrarCancionesPlaylist()
{
    // SE MOTRARIA EN CONSOLA
    int i = 1;

    void mostrarCancion(void *_, int idx, void *val)
    {
        Cancion *c = (Cancion *)val;
        printf("%d. %s - %s (%d segundos)\n", idx + 1, c->nombre, c->genero, c->duracion);
    }

    forEachInLista(playlist->canciones, mostrarCancion, NULL);
}