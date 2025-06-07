#ifndef CRUD_ARTISTAS_H
#define CRUD_ARTISTAS_H

#include "crud.h";

// Declaración de funciones:

Artista *crear(Artista, char *nombre);
Artista *leer(Artista, char *);
void actualizar(Artista, Artista *);
bool eliminar(Artista, Artista *);

#endif // CRUD_ARTISTAS_H