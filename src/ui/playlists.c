//FUNCIONES A CREAR, CREARPLAYLIST, AGREGARCANCIONPLAYLIST, ELIMINARCANCIONPLAYLIST, MOSTRARCANCIONESPLAYLIST, ELIMINARPLAYLIST
//CUANDO CREEN TODO, COMPILEN CON EL COMANDO CMAKE --BUILD --PRESET CONSOLE PARA VER SI NO HAY ERRORES O PRECAUCIONES

#include <ui/interfaces.h>
#include <utils/utils.h>

bool crearPlaylist(const char* nombre){
    Playlist* nueva= newPlaylist();
    if(!nueva) return false;

    nueva->nombre=strdup(nombre);
    nueva->canciones = newLista(NULL);

    return nueva;
}

bool agregarCancionPlaylist(Cancion* cancion, Playlist* playlist){
    // Verifica que la canción y la playlist no sean nulas
    // y que la lista de canciones de la playlist esté inicializada
    if(!cancion || !playlist || !playlist->canciones) return false;

    // Agrega la canción a la lista de canciones de la playlist
    agregarALista(playlist->canciones, cancion);
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