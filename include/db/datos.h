#ifndef VIBECAST_DATOS_H
#define VIBECAST_DATOS_H 1

#include <db/estructuras.h>
#include <stdbool.h>

/**
 * Variables globales
 * Estas variables estarán disponibles
 * para todos los archivos que incluyan
 * datos.h
 * #include <db/datos.h>
 */

extern ABB usuarios;
extern ABB artistas;
extern Lista canciones;
extern Cola anuncios;

/* ======== Declaración de funciones ======== */

// Función para cargar los datos en las estructuras
bool VibeCast_LoadData(char **errmsg);

// Función para liberar la memoria
bool VibeCast_FreeData();

/* Comparadores */

// Compara un usuario y un username
new_cmpfn(cmpUsuarioConUsername);

// Compara una cancion y un id
new_cmpfn(cmpCancionConId);

#endif // VIBECAST_DATOS_H