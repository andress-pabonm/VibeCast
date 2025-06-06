#ifndef CRUD_USUARIOS_H
#define CRUD_USUARIOS_H

#include "crud.h"
#include "estructuras.h"

// Declaración de funciones:

Usuario *crear(Usuario);
Lista leer(Usuario, char *);
void actualizar(Usuario, Usuario *);
void eliminar(Usuario, Usuario *);

#endif // CRUD_USUARIOS_H