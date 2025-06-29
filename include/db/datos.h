#ifndef VIBECAST_DATOS_H
#define VIBECAST_DATOS_H 1

#include <db/estructuras.h>

extern ABB usuarios;
extern ABB artistas;
extern Lista canciones;
extern Cola anuncios;

/* ======== Declaración de funciones ======== */

// Función para cargar los datos en las estructuras
bool func(LoadData);

// Función para liberar la memoria
bool func(FreeData);

/* Comparadores */

// Compara un usuario y un username
new_cmpfn(cmpUsuarioConUsername);

// Compara una cancion y un id
new_cmpfn(cmpCancionConId);

#endif // VIBECAST_DATOS_H