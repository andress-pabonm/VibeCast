#include <Utilidades/nodos.h>

// =======================
// Funciones para Lista
// =======================

/*
 * Inserta un nodo en una lista simple.
 * Si se proporciona una función de comparación, inserta el nodo en orden.
 * Si no, lo agrega al final.
 */
void insertarNodo(Lista, Lista *lista, NodoLista *nodo, cmpfn_t cmp)
{
    if (!lista || !nodo)
        return;

    NodoLista **ref = &lista->inicio;

    if (cmp)
    {
        // Inserción ordenada
        while (*ref && cmp((*ref)->valor, nodo->valor) < 0)
        {
            ref = &(*ref)->siguiente;
        }
    }
    else
    {
        // Inserción al final
        while (*ref)
        {
            ref = &(*ref)->siguiente;
        }
    }

    nodo->siguiente = *ref;
    *ref = nodo;
}

/*
 * Busca un nodo en la lista usando la función de comparación.
 * Devuelve el primer nodo que coincida con el valor buscado.
 */
NodoLista *buscarNodo(Lista, Lista *lista, void *valor, cmpfn_t cmp)
{
    if (!lista || !valor || !cmp)
        return NULL;

    NodoLista *nodo = lista->inicio;
    while (nodo)
    {
        if (!cmp(nodo->valor, valor))
            break;
        nodo = nodo->siguiente;
    }
    return nodo;
}

/*
 * Elimina un nodo específico de la lista.
 * No hace nada si el nodo no está en la lista.
 */
void eliminarNodo(Lista, Lista *lista, NodoLista *objetivo)
{
    if (!lista || !objetivo)
        return;

    NodoLista **ref = &lista->inicio;
    while (*ref && *ref != objetivo)
        ref = &(*ref)->siguiente;
    if (!*ref)
        return;

    *ref = objetivo->siguiente;
    free(objetivo);
}

/*
 * Libera todos los nodos de la lista.
 * Establece el inicio en NULL después de limpiar.
 */
void liberar(Lista, Lista *lista, freefn_t freevalue)
{
    if (!lista)
        return;

    NodoLista *actual = lista->inicio;

    if (freevalue)
    {
        while (actual)
        {
            NodoLista *tmp = actual;
            actual = actual->siguiente;
            freevalue(tmp->valor);
            free(tmp);
        }
    }
    else
    {
        while (actual)
        {
            NodoLista *tmp = actual;
            actual = actual->siguiente;
            free(tmp);
        }
    }

    lista->inicio = NULL;
}

// =======================
// Funciones para Pila
// =======================

/*
 * Inserta un nodo en la cima de la pila.
 */
void insertarNodo(Pila, Pila *pila, NodoPila *nodo)
{
    if (!pila || !nodo)
        return;
    nodo->debajo = pila->cima;
    pila->cima = nodo;
}

/*
 * Elimina y devuelve el valor del nodo en la cima.
 * Retorna NULL si la pila está vacía.
 */
void *eliminarNodo(Pila, Pila *pila)
{
    if (!pila || !pila->cima)
        return NULL;

    NodoPila *cima = pila->cima;
    void *valor = cima->valor;
    pila->cima = cima->debajo;
    free(cima);
    return valor;
}

/*
 * Libera todos los nodos de la pila.
 */
void liberar(Pila, Pila *pila, freefn_t freevalue)
{
    if (!pila)
        return;

    if (freevalue)
    {
        while (pila->cima)
            freevalue(eliminarNodo(Pila, pila));
    }
    else
    {
        while (pila->cima)
            eliminarNodo(Pila, pila);
    }
}

// =======================
// Funciones para Cola
// =======================

/*
 * Inserta un nodo al final de la cola.
 */
void insertarNodo(Cola, Cola *cola, NodoCola *nodo)
{
    if (!cola || !nodo)
        return;

    if (cola->fin)
        cola->fin->siguiente = nodo;
    cola->fin = nodo;
    if (!cola->inicio)
        cola->inicio = nodo;
}

/*
 * Elimina y devuelve el valor del nodo en el frente.
 * Retorna NULL si la cola está vacía.
 */
void *eliminarNodo(Cola, Cola *cola)
{
    if (!cola || !cola->inicio)
        return NULL;

    NodoCola *inicio = cola->inicio;
    void *valor = inicio->valor;
    cola->inicio = inicio->siguiente;
    if (!cola->inicio)
        cola->fin = NULL;
    free(inicio);
    return valor;
}

/*
 * Libera todos los nodos de la cola.
 */
void liberar(Cola, Cola *cola, freefn_t freevalue)
{
    if (!cola)
        return;

    if (freevalue)
    {
        while (cola->inicio)
            freevalue(eliminarNodo(Cola, cola));
    }
    else
    {
        while (cola->inicio)
            eliminarNodo(Cola, cola);
    }
}

// =======================
// Funciones para ABB
// =======================

/*
 * Inserta un nodo en un árbol binario de búsqueda.
 * No permite nodos duplicados (según la comparación).
 */
void insertarNodo(ABB, ABB *abb, NodoABB *nodo, cmpfn_t cmp)
{
    if (!abb || !nodo || !cmp)
        return;

    NodoABB **ref = &abb->raiz;
    while (*ref)
    {
        int cmpv = cmp((*ref)->valor, nodo->valor);
        if (!cmp)
            return; // Nodo duplicado
        ref = (cmp > 0) ? &(*ref)->izq : &(*ref)->der;
    }
    *ref = nodo;
}

/*
 * Busca un nodo en el ABB usando la función de comparación.
 * Devuelve un puntero al nodo si lo encuentra, NULL en caso contrario.
 */
NodoABB *buscarNodo(ABB, ABB *abb, void *valor, cmpfn_t cmp)
{
    if (!abb || !valor || !cmp)
        return NULL;

    NodoABB *nodo = abb->raiz;
    while (nodo)
    {
        int cmpv = cmp(nodo->valor, valor);
        if (!cmp)
            break;
        nodo = (cmp > 0) ? nodo->izq : nodo->der;
    }
    return nodo;
}

/*
 * Elimina un nodo hoja del ABB.
 * No elimina si el nodo tiene hijos.
 */
void eliminarNodo(ABB, ABB *abb, NodoABB *objetivo, cmpfn_t cmp)
{
    if (!abb || !objetivo || !cmp)
        return;
    if (objetivo->izq || objetivo->der)
        return;

    NodoABB **ref = &abb->raiz;
    while (*ref)
    {
        if (*ref == objetivo)
            break;

        int cmpv = cmp((*ref)->valor, objetivo->valor);
        if (!cmp)
            return;
        ref = (cmp > 0) ? &(*ref)->izq : &(*ref)->der;
    }

    if (!*ref)
        return;
    *ref = NULL;
    free(objetivo);
}

void liberarRecursivo(NodoABB *nodo, freefn_t freevalue)
{
    if (!nodo)
        return;
    liberarRecursivo(nodo->izq, freevalue);
    liberarRecursivo(nodo->der, freevalue);
    if (freevalue)
        freevalue(nodo->valor);
    free(nodo);
}

/*
 * Libera todos los nodos del árbol de manera recursiva.
 */
void liberar(ABB, ABB *abb, freefn_t freevalue)
{
    if (!abb)
        return;

    liberarRecursivo(abb->raiz, freevalue);
    abb->raiz = NULL;
}
