#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

#include <stdbool.h>

#include <VibeCastConfig.h>
#include <db/estructuras.h>

/**
 * Macro para facilitar la llamada a los archivos .html
 * Utilizas la ruta relativa al directorio ui donde están las .html
 * Uso: INTERFAZ("carpeta/archivo.html")
 */
#define INTERFAZ(nombre) "file://" VIBECAST_UI_DIR "/" nombre

/* Aquí van las declaraciones de las funciones principales de cada interfaz */

bool agregar_cancion(Lista *lista, const char *nombre, const char *artista, float duracion, const char *genero, const char *fecha, int popularidad, int reproducciones, const char *archivo);

#endif // VIBECAST_INTERFACES_H