#include <utils/nodos/nodo.h>

#include <utils/memmgr.h>
#include <utils/macros.h>
#include <stddef.h>

// ================================
// Funciones para Nodo
// ================================

Nodo newNodo(void *value_ptr)
{
    return value_ptr
               ? alloc(struct Nodo,
                       &cast(struct Nodo,
                             .l = NULL,
                             .r = NULL,
                             .v = value_ptr))
               : NULL;
}

void destroyNodo(Nodo nodo)
{
    freem(nodo);
}
