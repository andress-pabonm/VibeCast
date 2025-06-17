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

void crearPlaylist(const char *nombrePlaylist, const char *fechaCreacion, Lista *caniones);
void agregarCancion(const char *nombrePlaylist);

#endif // VIBECAST_INTERFACES_H