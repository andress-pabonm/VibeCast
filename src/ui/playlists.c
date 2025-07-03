#include <ui/interfaces.h>
#include <utils/utils.h>

bool crearPlaylist(const char* nombre){
    Playlist* nueva= newPlaylist();
    if(!nueva) return NULL;

    nueva->nombre=strdup(nombre);
    nueva->canciones = newLista(NULL);

    return nueva;
}