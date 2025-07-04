#include <ui/interfaces.h>
#include <utils/utils.h>

static new_cmpfn(cmpPlaylistConNombre)
{
    const Playlist *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

new_operfn(restar_popularidad)
{
    Cancion *cancion = val;
    cancion->popularidad--;
    return FOREACH_CONTINUE; // constante que indica que se sigue iterando con el bucle de reccorer la lista
}

new_operfn(mostrar_canciones)
{
    Cancion *cancion = val; // Canción actual

    printf("[%d]: %s\n", idx, cancion->nombre); // Muestra cancion actual

    return FOREACH_CONTINUE; // Para seguir con el bucle
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

    printf("Canción %s eliminada de la playlist '%s'\n", cancionEliminada->nombre, playlist->nombre);
    // Restamos la popularidad de la canción eliminada
    cancionEliminada->popularidad--;

    return true;
}

bool eliminarPlaylist(const char *nombrePlaylist)
{
    // Buscamos la playlist por nombre
    Playlist *playlisteliminada = deleteValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre); // Eliminamos la playlist de lalista de playlists
    if (playlisteliminada == NULL)
    {
        printf("Error: No se pudo eliminar la playlist '%s'\n", nombrePlaylist);
        return false;
    }

    printf("Playlist '%s' eliminada correctamente\n", nombrePlaylist);
    // Liberamos la memoria asociada a la playlist
    freem(playlisteliminada->nombre); // Liberamos el nombre de la playlist

    destroyLista(playlisteliminada->canciones, restar_popularidad, NULL); // Eliminamos la playlist y todas las canciones eliminamos una popularidad

    freem(playlisteliminada);

    return true;
}

void mostrarCancionesPlaylist(const char *nombrePlaylist)
{
    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre); // Buscamos la playlist por nombre

    if (!playlist)
    {
        printf("Error: La playlist '%s' no existe\n", nombrePlaylist);

        return; // Retornar para evitar errores con las siguientes instrucciones
    }

    printf("Playlist: %s\n", playlist->nombre);

    forEachInLista(playlist->canciones, mostrar_canciones, NULL);
}