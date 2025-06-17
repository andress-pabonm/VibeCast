#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <db/estructuras.h>
// #include <utils/nodos.h> // Ya está incluido en estructuras.h
#include <ui/interfaces.h>

bool agregar_cancion(Lista *lista, const char *nombre, const char *artista, float duracion, const char *genero, const char *fecha, int popularidad, int reproducciones, const char *archivo){
    Cancion *nueva_cancion = malloc(sizeof(Cancion));

    if (!nueva_cancion) return false;

    nueva_cancion->nombreCancion = strdup("idol");
    nueva_cancion->artista = malloc(sizeof(Artista)); //Inicializar el artista
    strcpy(nueva_cancion->artista->nombreArtista, "Yoasobi");
    nueva_cancion->duracion = 3.33;
    nueva_cancion->genero = strdup("sepadios");
    strncpy(nueva_cancion->fechaLanzamiento, "nose", sizeof(nueva_cancion->fechaLanzamiento));
    nueva_cancion->popularidad = 2;
    nueva_cancion->reproducciones = 8000;
    nueva_cancion->archivo = strdup("canciones/idol.mp3");

    // Insertar la canción en la lista
    insertarNodo(Lista, lista, nueva_cancion, NULL);

    printf("Canción '%s' de '%s' agregada correctamente.\n", nueva_cancion->nombreCancion, nueva_cancion->artista->nombreArtista);

    return true;
}