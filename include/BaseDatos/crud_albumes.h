#ifndef CRUD_ALBUMES_H
#define CRUD_ALBUMES_H

#include "crud.h";

// Declaración de funciones:

Album *crear(Album, char *nombre, char *fecha, Lista *canciones);
Lista leer(Album, char *);
void actualizar(Album, Album *);
void eliminar(Album, Album *);

#endif // CRUD_ALBUMES_H