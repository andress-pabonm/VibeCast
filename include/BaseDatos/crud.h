#ifndef CRUD_H
#define CRUD_H

#include "estructuras.h";
#include "liberadores.h";
#include <stdbool.h>

// Macros para facilitar la declaración, definición y llamada a las funciones CRUD:

#define crear(type, ...) crear ## type (__VA_ARGS__)
#define leer(type, ...) leer ## type (__VA_ARGS__)
#define actualizar(type, ...) actualizar ## type (__VA_ARGS__)
#define eliminar(type, ...) eliminar ## type (__VA_ARGS__)

#endif // CRUD_H
