#include <ui/interfaces.h>

void generarTop5Canciones(){
    int longitud;

    FILE *archivo;
    archivo = newfile("top5_canciones.txt", NULL);

    fpringtf(archivo, "Top 5 Canciones más reproducidas:\n");

    longitud = getListaLength(usuario->playlists->canciones);
}