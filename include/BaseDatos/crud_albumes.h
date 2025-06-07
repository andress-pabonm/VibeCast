#ifndef CRUD_ALBUMES_H
#define CRUD_ALBUMES_H

#include "crud.h";

// Declaración de funciones:

Album *crear(Album, char *nombre, char *fecha, Lista *canciones);
Album *leer(Album, char *);
void actualizar(Album, Album *);
bool eliminar(Album, Album *);

#endif // CRUD_ALBUMES_H