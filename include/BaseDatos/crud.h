#ifndef CRUD_H
#define CRUD_H

// Macros para facilitar la declaración, definición y llamada a las funciones CRUD:

#define crear(type, ...) create ## type (__VA_ARGS__)
#define leer(type, ...) read ## type (__VA_ARGS__)
#define actualizar(type, ...) update ## type (__VA_ARGS__)
#define eliminar(type, ...) delete ## type (__VA_ARGS__)

#endif // CRUD_H
