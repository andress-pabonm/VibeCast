#include <utils/nodos/cola.h>
#include <utils/nodos/nodo.h>

#include <utils/memmgr.h>
#include <utils/macros.h>
#include <stddef.h>

// ========================================
// Definición interna
// ========================================

typedef struct __Cola
{
    Nodo head;
    Nodo tail;
} *__Cola;

// ========================================
// API Pública - Creación
// ========================================

Cola newCola()
{
    return alloc(struct __Cola,
                 &cast(struct __Cola,
                       .head = NULL,
                       .tail = NULL));
}

// ========================================
// API Pública - Inserción
// ========================================

bool insertValueInCola(Cola cola, void *value_ptr)
{
    if (!cola)
        return false;

    Nodo nodo = newNodo(value_ptr);
    if (!nodo)
        return false;

    __Cola __cola = cola;

    if (!__cola->head)
        __cola->head = nodo;

    if (__cola->tail)
        __cola->tail->r = nodo;

    __cola->tail = nodo;

    return true;
}

// ========================================
// API Pública - Eliminación
// ========================================

void *deleteValueInCola(Cola cola)
{
    if (!cola)
        return NULL;

    __Cola __cola = cola;

    if (!__cola->head)
        return NULL;

    Nodo nodo = __cola->head;
    __cola->head = nodo->r;

    if (!__cola->head)
        __cola->tail = NULL;

    void *value_ptr = nodo->v;
    destroyNodo(nodo);

    return value_ptr;
}

// ========================================
// API Pública - Destrucción
// ========================================

void destroyCola(Cola cola, operfn_t callback, void *arg)
{
    if (!cola)
        return;

    __Cola __cola = cola;

    if (callback)
    {
        void *value_ptr;

        while (__cola->head)
        {
            value_ptr = deleteValueInCola(cola);
            callback(arg, 0, value_ptr);
        }
    }
    else
    {
        while (__cola->head)
            deleteValueInCola(cola);
    }

    freem(cola);
}
