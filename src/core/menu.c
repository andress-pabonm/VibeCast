#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <db/estructuras.h>
#include <utils/nodos.h>
#include <ui/interfaces.h>

//Hablar la logica de crear cancion por cancion siendo admin

//Cada vez que el usuario quiera crear una playlist, se le pedira el nombre de la playlist
void crearPlaylist(const char *nombrePlaylist, const char *fechaCreacion, Lista *canciones)
{
    if (!nombrePlaylist || !fechaCreacion || !canciones)
    {
        printf("Error: Parámetros inválidos para crear la playlist\n");
        return;
    }

    Playlist *nuevaPlaylist = malloc(sizeof(Playlist));

    if (!nuevaPlaylist)
    {
        printf("Error: No se pudo asignar memoria para la nueva playlist\n");
        return;
    }

    nuevaPlaylist -> nombrePlaylist = strdup(nombrePlaylist); //Varialbe q se veria desde el html y el js
    nuevaPlaylist->fechaCreacion[0] = '\0'; //xd con el salto de linea
    strncat(nuevaPlaylist->fechaCreacion, fechaCreacion, sizeof(nuevaPlaylist->fechaCreacion) - 1); //fecha cogia del computador
    //nuevaPlaylist->canciones = *canciones; //apunto a una sola cancion desea por el usuario, depende si el usuario quiere agregar canciones a la playlist o no 

    insertarNodo(Lista, &nuevaPlaylist->canciones, nuevaPlaylist, NULL); // Inserta la nueva playlist en la lista de canciones preguntar bien como es la llamada de las funciones

    // Aquí podrías agregar la nueva playlist a una lista global o base de datos
} 

void agregarCancion(const char *nombrePlaylist) //Escoge la playlist en tema grafico
{
    if (!nombrePlaylist)
    {
        printf("Error: Parámetros inválidos para agregar la canción\n");
        return;
    }

    // Aquí deberías buscar la playlist por nombre y luego agregar la canción a esa playlist
    // Por simplicidad, asumimos que la playlist ya existe y se encuentra en una lista global o base de datos

    // Ejemplo de cómo podrías buscar la playlist y agregar la canción
    Playlist *playlist = buscarNodo(nombrePlaylist); // Implementa esta función según tu lógica

    if (playlist)
    {
        
        playlist->canciones.head = NULL; // Aqui logica de q la playlist a apunta a una cancion


    }
    else
    {
        printf("Error: Playlist '%s' no encontrada\n", nombrePlaylist);
    }
}