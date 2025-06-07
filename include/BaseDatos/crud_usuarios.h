#ifndef CRUD_USUARIOS_H
#define CRUD_USUARIOS_H

#include "crud.h";

// Declaración de funciones:

Usuario *crear(Usuario, char *email, char *password);
Lista leer(Usuario, void *valor, cmpfn_t cmp);
void actualizar(Usuario, Usuario *usuario);
void eliminar(Usuario, Usuario *usuario);

#endif // CRUD_USUARIOS_H