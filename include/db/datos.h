#ifndef DATOS_H
#define DATOS_H

#include <VibeCastConfig.h>
#include <db/dbmgr.h>
#include <db/estructuras.h>

#include <stdbool.h>

extern ABB usuarios;
extern ABB artistas;
extern Lista canciones;
extern Cola anuncios;

/* ======== Declaración de funciones ======== */

// Función para cargar los datos en las estructuras
bool func(LoadData);

// ============
// Utilidades
// ============

// Compara dos usuarios
new_cmp(cmpUsuarios);

// Compara un usuario y un username
new_cmp(cmpUsuarioConUsername);

// Compara dos artistas
new_cmp(cmpArtistas);

// Compara una cancion y un id
new_cmp(cmpCancionConId);

#endif // DATOS_H