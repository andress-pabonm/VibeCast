#include <utils/nodos.h>
#include <stdlib.h>

static Nodo *nuevoNodo(void *value_ptr)
{
    Nodo *nodo = value_ptr ? malloc(sizeof(Nodo)) : NULL;

    if (nodo)
    {
        nodo->izq = NULL;
        nodo->der = NULL;
        nodo->value_ptr = value_ptr;
    }

    return nodo;
}

// =======================
// Funciones para Lista
// =======================

void forEachIn(Lista, Lista *lista, opfn_t op)
{
    int i = 0;
    Nodo *nodo = lista->head;

    while (nodo && op(i, nodo->value_ptr))
    {
        i++;
        nodo = nodo->der;
    }
}

bool insertarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp)
{
    if (!lista)
        return false;

    Nodo *nodo = nuevoNodo(value_ptr);

    if (!nodo)
        return false;

    Nodo **ref = &lista->head;

    if (cmp)
    {
        while (*ref && cmp((*ref)->value_ptr, value_ptr) < 0)
            ref = &(*ref)->der;
    }
    else
    {
        while (*ref)
            ref = &(*ref)->der;
    }

    nodo->der = *ref;
    *ref = nodo;

    return true;
}

Nodo **buscarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp)
{
    if (!lista || !value_ptr || !cmp)
        return NULL;

    Nodo **ref = &lista->head;

    while (*ref && cmp((*ref)->value_ptr, value_ptr))
        ref = &(*ref)->der;

    return ref;
}

bool eliminarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp)
{
    Nodo **ref = buscarNodo(Lista, lista, value_ptr, cmp);

    if (!ref)
        return false;

    Nodo *nodo = *ref; // Guardar el puntero temporalmente
    *ref = nodo->der;  // Aislar el nodo y unir la cadena rota
    free(nodo);        // Liberar la memoria del nodo

    return true;
}

bool liberar(Lista, Lista *lista, freefn_t free_value)
{
    if (!lista)
        return false;

    Nodo *nodo = lista->head;
    Nodo *tmp;

    if (free_value)
    {
        while (nodo)
        {
            if (!free_value(nodo->value_ptr))
                return false; // No se pudo liberar la memoria del valor

            tmp = nodo;
            nodo = nodo->der;
            free(tmp);
        }
    }
    else
    {
        while (nodo)
        {
            tmp = nodo;
            nodo = nodo->der;
            free(tmp);
        }
    }

    lista->head = NULL;

    return true;
}

// =======================
// Funciones para Pila
// =======================

bool insertarNodo(Pila, Pila *pila, void *value_ptr)
{
    if (!pila)
        return false;

    Nodo *nodo = nuevoNodo(value_ptr);

    if (!nodo)
        return false;

    nodo->izq = pila->top; // Enlazar el nuevo nodo
    pila->top = nodo;      // Colocarlo en la cima

    return true;
}

void *eliminarNodo(Pila, Pila *pila)
{
    if (!pila || !pila->top)
        return NULL;

    Nodo *top = pila->top; // Guardar el nodo temporalmente
    pila->top = top->izq;  // Actualizar la cima de la pila

    void *value_ptr = top->value_ptr; // Guardar el valor
    free(top);                        // Liberar la memoria

    return value_ptr; // Retornar el valor
}

bool liberar(Pila, Pila *pila, freefn_t free_value)
{
    if (!pila)
        return false;

    if (free_value)
    {
        while (pila->top)
        {
            if (!free_value(eliminarNodo(Pila, pila)))
                return false;
        }
    }
    else
    {
        while (pila->top)
            eliminarNodo(Pila, pila);
    }

    return true;
}

// =======================
// Funciones para Cola
// =======================

bool insertarNodo(Cola, Cola *cola, void *value_ptr)
{
    if (!cola)
        return false;

    Nodo *nodo = nuevoNodo(value_ptr);

    if (!nodo)
        return false;

    if (cola->tail)             // Si está definido el puntero a la cola
        cola->tail->der = nodo; // Insertar el nuevo nodo después
    cola->tail = nodo;          // Acutalizar el puntero a la cola

    if (!cola->head)       // En caso de que la cola esté vacia
        cola->head = nodo; // Actualizar el puntero a la cabeza

    return true;
}

void *eliminarNodo(Cola, Cola *cola)
{
    if (!cola || !cola->head)
        return NULL;

    Nodo *head = cola->head; // Guardar el nodo temporalmente
    cola->head = head->der;  // Actualizar el puntero a la cabeza

    if (!cola->head)       // En caso de que la cola esté vacia
        cola->tail = NULL; // Actualizar el puntero a la cola

    void *value_ptr = head->value_ptr; // Guardar el valor
    free(head);                        // Liberar el espacio en memoria

    return value_ptr; // Retornar el valor
}

bool liberar(Cola, Cola *cola, freefn_t free_value)
{
    if (!cola)
        return false;

    if (free_value)
    {
        while (cola->head)
        {
            if (!free_value(eliminarNodo(Cola, cola)))
                return false;
        }
    }
    else
    {
        while (cola->head)
            eliminarNodo(Cola, cola);
    }

    return true;
}

// =======================
// Funciones para ABB
// =======================

bool insertarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp)
{
    if (!abb || !cmp)
        return false;

    Nodo *nodo = nuevoNodo(value_ptr);

    if (!nodo)
        return false;

    Nodo **ref = &abb->root;
    int cmpv;

    while (*ref)
    {
        cmpv = cmp((*ref)->value_ptr, value_ptr);

        if (!cmp) // Valor ya existente en el árbol
        {
            free(nodo); // Liberar la memoria del nodo
            return false;
        }

        // Desplazarse a la izquierda o derecha
        ref = (cmpv > 0) ? &(*ref)->izq : &(*ref)->der;
    }

    *ref = nodo; // Enlazar el nuevo nodo

    return true;
}

Nodo **buscarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp)
{
    if (!abb || !value_ptr || !cmp)
        return NULL;

    Nodo **ref = &abb->root;
    int cmpv;

    while (*ref && (cmpv = cmp((*ref)->value_ptr, value_ptr)))
    {
        // Desplazarse a la izquierda o a la derecha
        ref = (cmp > 0) ? &(*ref)->izq : &(*ref)->der;
    }

    return ref;
}

bool eliminarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp)
{
    Nodo **ref = buscarNodo(ABB, abb, value_ptr, cmp);

    if (!ref)
        return false;

    Nodo *nodo = *ref;

    if (nodo->izq || nodo->der)
        return false; // El nodo no es una hoja

    *ref = NULL; // Aislar el nodo hoja
    free(nodo);  // Liberar la memoria

    return true;
}

static bool liberarABBRecursivo(Nodo *root, freefn_t free_value)
{
    if (root)
    {
        if (!liberarABBRecursivo(root->izq, free_value) ||
            !liberarABBRecursivo(root->der, free_value))
            return false;

        if (free_value && !free_value(root->value_ptr))
            return false;

        free(root);
    }

    return true;
}

bool liberar(ABB, ABB *abb, freefn_t free_value)
{
    if (!abb)
        return false;

    if (!liberarABBRecursivo(abb->root, free_value))
        return false; // No se pudo liberar la memoria

    abb->root = NULL;

    return true;
}

/* PreOrder */
static bool ABB_PreOrder(int depth, Nodo *root, opfn_t op)
{
    return !root ||                                   // Cuando llegue a NULL retorna true para continuar
           !op(depth, root->value_ptr) ||             // Primero evalúa la raiz
           !ABB_PreOrder(depth + 1, root->izq, op) || // Luego evalúa la rama izquierda
           ABB_PreOrder(depth + 1, root->der, op);    // Y por último la rama derecha
}

void forEachIn(ABB_PreOrder, ABB *abb, opfn_t op)
{
    ABB_PreOrder(0, abb->root, op);
}

/* InOrder */
static bool ABB_InOrder(int depth, Nodo *root, opfn_t op)
{
    return !root ||                                   // Cuando llegue a NULL retorna true para continuar
           !ABB_PreOrder(depth + 1, root->izq, op) || // Primero evalúa la rama izquierda
           !op(depth, root->value_ptr) ||             // Luego evalúa la raiz
           ABB_PreOrder(depth + 1, root->der, op);    // Y por último evalúa la rama derecha
}

void forEachIn(ABB_InOrder, ABB *abb, opfn_t op)
{
    ABB_InOrder(0, abb->root, op);
}

/* PostOrder */
static bool ABB_PostOrder(int depth, Nodo *root, opfn_t op)
{
    return !root ||                                   // Cuando llegue a NULL retorna true para continuar
           !ABB_PreOrder(depth + 1, root->izq, op) || // Primero evalúa la rama izquierda
           !ABB_PreOrder(depth + 1, root->der, op) || // Luego evalú la rama derecha
           op(depth, root->value_ptr);                // Y por último evalúa la raiz
}

void forEachIn(ABB_PostOrder, ABB *abb, opfn_t op)
{
    ABB_PostOrder(0, abb->root, op);
}
