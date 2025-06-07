#ifndef CRUD_USUARIOS_H
#define CRUD_USUARIOS_H

#include "crud.h";

// Declaración de funciones:

Usuario *crear(Usuario, char *email, char *password, char *nombre, char *nickname, char *pais, Plan plan);
Usuario *leer(Usuario, void *valor, cmpfn_t cmp);
void actualizar(Usuario, Usuario *usuario);
bool eliminar(Usuario, Usuario *usuario);

#endif // CRUD_USUARIOS_H