#include <utils/nodos/pila.h>

#include <utils/memmgr.h>
#include <utils/macros.h>

#include <stddef.h>

// ========================================
// Estructuras internas
// ========================================

typedef struct Nodo
{
    struct Nodo *l;
    void *v;
} *Nodo;

typedef struct __Pila
{
    Nodo top;
} *__Pila;

// ========================================
// Funciones auxiliares
// ========================================

static Nodo newNodo(void *value_ptr)
{
    return value_ptr
               ? alloc(
                     struct Nodo,
                     &cast(
                         struct Nodo,
                         .l = NULL,
                         .v = value_ptr))
               : NULL;
}

static void destroyNodo(Nodo nodo)
{
    freem(nodo);
}

// ========================================
// API Pública - Creación
// ========================================

Pila newPila()
{
    return alloc(struct __Pila,
                 &cast(struct __Pila,
                       .top = NULL));
}

// ========================================
// API Pública - Inserción
// ========================================

bool insertValueInPila(Pila pila, void *value_ptr)
{
    if (!pila)
        return false;

    Nodo nodo = newNodo(value_ptr);
    if (!nodo)
        return false;

    __Pila __pila = pila;
    nodo->l = __pila->top; // El nuevo nodo apunta al anterior top
    __pila->top = nodo;    // Actualizar top al nuevo nodo
    return true;
}

// ========================================
// API Pública - Eliminación
// ========================================

void *deleteValueInPila(Pila pila)
{
    if (!pila)
        return NULL;

    __Pila __pila = pila;
    if (!__pila->top)
        return NULL;

    Nodo nodo = __pila->top;
    __pila->top = nodo->l;

    void *value_ptr = nodo->v;
    destroyNodo(nodo);
    return value_ptr;
}

// ========================================
// API Pública - Destrucción
// ========================================

void destroyPila(Pila pila, operfn_t callback, void *arg)
{
    if (!pila)
        return;

    __Pila __pila = pila;

    if (callback)
    {
        while (__pila->top)
        {
            void *value_ptr = deleteValueInPila(pila);
            callback(arg, 0, value_ptr);
        }
    }
    else
    {
        while (__pila->top)
            deleteValueInPila(pila);
    }

    freem(pila);
}
