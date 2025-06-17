/* Aquí se cargan los datos desde la base datos y se los organiza en las diferentes estructuras. */

#include <db/estructuras.h>
#include <stddef.h>

/**
 * 1. Cargar toda la información a la vez (usuarios, artistas, álbumes y canciones)
 * 2. Crear los usuarios y añadirlos a un árbol binario
 * 3. Por cada usuario: Crear los perfiles de artista, enlazarlos y añadirlos a un árbol
 * 4. Por cada artista: Crear los álbumes, enlazarlos y añadirlos a una lista
 * 5. Por cada álbum: Crear las canciones, enlazarlas y añadirlas en una lista
 *
 * Nota: Como las canciones no tienen una referencia a qué álbum pertenecen,
 * sería de realizar primero el paso 4 y luego el 3, creo.
 */

// static ABB usuarios = {NULL};
// static ABB artistas = {NULL};
// static Lista albumes = {NULL};
// static Lista canciones = {NULL};