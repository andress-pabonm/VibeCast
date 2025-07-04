#include <ui/interfaces.h>
#include <utils/utils.h>

static new_cmpfn(cmpPlaylistConNombre)
{
    const Playlist *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

bool crearPlaylist(const char* nombre){
    Playlist *nuevaPlaylist = newPlaylist(); // Inicaliza como null

    if(!nuevaPlaylist) return false;

    nuevaPlaylist->nombre = asprintf(nombre);

    insertValueInLista(usuario->playlists, nuevaPlaylist); // Insertamos las playlist en lista

    return true;
}

bool agregarCancionPlaylist(const char *nombrePlaylist, const char *nombreCanciones){
    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre);

    if(!playlist){
        prinf("Error: La playlist '%s' no existe\n", nombrePlaylist);

        return false;
    }

    Cancion *cancion = newCancion(); // Inicaliza como null

    cancion->nombre = asprintf(nombreCanciones);

    // Agrega la canción a la lista de canciones de la playlist
    insertValueInLista(playlist->canciones, cancion);
    return true;
}

void agregarALista(Lista lista, Cancion* cancion){
    if (!lista || !cancion) return;  

    Lista* nuevoNodo = newLista(cancion);

    Lista* actual = lista->start;

    if (!actual) {
        lista->start = nuevoNodo; // Si la lista está vacía, el nuevo nodo es el inicio
        return;

    }else{
        while (actual->next) {
            actual = actual->next; // Recorre hasta el final de la lista
        }
        actual->next = nuevoNodo; // Agrega el nuevo nodo al final
    }

    cancion->popularidad++;

    // Esta función debería agregar la canción a la lista
    // Implementación de agregar a la lista
}




bool eliminarCancionPlaylist(Cancion* cancion, Playlist* playlist){
    //SE PODRIA HACER CON UNA FUNCION QUE RECIBA EL ID DE LA CANCION A ELIMINAR
    //Y LA PLAYLIST EN LA QUE SE QUIERE ELIMINAR
    if(!cancion || !playlist || !playlist->canciones) return false;

    Cancion* actual = playlist->canciones->start;
}

bool eliminarPlaylist(){

}

void mostrarCancionesPlaylist(){
    //SE MOTRARIA EN CONSOLA
}