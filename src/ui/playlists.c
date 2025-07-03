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

bool agregarCancionPlaylist(){
    //CREO YO QUE PARA NO CREAR TANTAS FUNCIONES HACER EN UNA SOLA, POR EJEMPLO
    //EN ESTA FUNCION SE PODRIA AGREGAR CANCION EN UNA PLAYLIST PERO TAMBIEN PODRIA ELIMINARLA
    //ASI NO CREAR DOS FUNCIONES
}

bool eliminarPlaylist(){

}

void mostrarCancionesPlaylist(){
    //SE MOTRARIA EN CONSOLA
}