#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

/* Aquí van las declaraciones de las funciones principales de cada interfaz */

bool agregar_cancion(Lista *lista, const char *nombre, const char *artista, float duracion, const char *genero, const char *fecha, int popularidad, int reproducciones, const char *archivo);
#endif // VIBECAST_INTERFACES_H