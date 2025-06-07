#ifndef CRUD_CANCIONES_H
#define CRUD_CANCIONES_H

#include "crud.h";

// Declaración de funciones:

Cancion *crear(Cancion, char *nombre, Artista *artista, int duracion, char *archivo);
Cancion *leer(Cancion, char *);
void actualizar(Cancion, Cancion *);
bool eliminar(Cancion, Cancion *);

#endif // CRUD_CANCIONES_H