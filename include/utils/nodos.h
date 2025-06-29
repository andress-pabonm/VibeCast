#ifndef NODOS_H
#define NODOS_H 1

#include <utils/utils.h>

// Representación de un nodo genérico.
typedef struct Nodo
{
    // Puntero al nodo a la izquierda.
    struct Nodo *izq;

    // Puntero al nodo a la derecha.
    struct Nodo *der;

    // Puntero a un valor.
    void *value_ptr;
} Nodo;

// Representación de una lista ligada.
typedef struct
{
    // Puntero al primer nodo de la lista.
    Nodo *head;
} Lista;

// Representación de un pila (stack).
typedef struct
{
    // Puntero al nodo en la cima de la pila.
    Nodo *top;
} Pila;

// Representación de una cola FIFO.
typedef struct
{
    // Puntero al primer nodo.
    Nodo *head;

    // Puntero al último nodo.
    Nodo *tail;
} Cola;

// Representación de un árbol binario de búsqueda.
typedef struct
{
    // Puntero a la raiz del árbol.
    Nodo *root;
} ABB;

/* ================ Declaración de funciones ================ */ 

// Para crear un nodo aislado con un valor
Nodo *nuevoNodo(void *value_ptr);

/**
 * Para facilitar la declaración, definición y llamada de funciones tipo insertarNodo.
 * @param type: Lista, Pila, Cola o ABB.
 * @param ...: Parámetros adicionales.
 */
#define insertarNodo(type, ...) insertarNodo##type(__VA_ARGS__)

/**
 * Para facilitar la declaración, definición y llamada de funciones tipo buscarNodo.
 * @param type: Lista, Pila, Cola o ABB.
 * @param ...: Parámetros adicionales.
 */
#define buscarNodo(type, ...) buscarNodo##type(__VA_ARGS__)

/**
 * Para facilitar la declaración, definición y llamada de funciones tipo eliminarNodo.
 * @param type: Lista, Pila, Cola o ABB.
 * @param ...: Parámetros adicionales.
 */
#define eliminarNodo(type, ...) eliminarNodo##type(__VA_ARGS__)

/**
 * Para facilitar la declaración, definición y llamada de funciones tipo liberar.
 * @param type: Lista, Pila, Cola o ABB.
 * @param ...: Parámetros adicionales.
 */
#define liberar(type, ...) liberar##type(__VA_ARGS__)

/**
 * Para facilitar la declaración, definición y llamada de funciones tipo forEachIn.
 * @param type: Lista, Pila, Cola o ABB.
 * @param ...: Parámetros adicionales.
 */
#define forEachIn(type, ...) forEachIn##type(__VA_ARGS__)

// ================================
// Funciones para Lista
// ================================

// Para insertar un valor en una lista.
bool insertarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
// Para buscar un valor en una lista.
Nodo **buscarNodo(Lista, Lista *lista, const void *value_ptr, cmpfn_t cmp);
// Para eliminar un valor en una lista.
bool eliminarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
// Para liberar la memoria de una lista.
bool liberar(Lista, Lista *lista, freefn_t free_value);
// Para recorrer una lista y operar sobre sus valores.
void forEachIn(Lista, Lista *lista, opfn_t op);

// ================================
// Funciones para Pila
// ================================

// Para insertar un valor en una pila.
bool insertarNodo(Pila, Pila *pila, void *value_ptr);
// Para eliminar un nodo en una pila.
void *eliminarNodo(Pila, Pila *pila);
// Para liberar la memoria de una pila.
bool liberar(Pila, Pila *pila, freefn_t free_value);
// Para operar los valores en una pila
// void forEachIn(Pila, Pila *pila, opfn_t op);

// ================================
// Funciones para Cola
// ================================

// Para insertar un valor en una cola.
bool insertarNodo(Cola, Cola *cola, void *value_ptr);
// Para eliminar un valor en una cola.
void *eliminarNodo(Cola, Cola *cola);
// Para liberar la memoria de una cola.
bool liberar(Cola, Cola *cola, freefn_t free_value);
// Para operar sobre los valores en una cola.
// void forEachIn(Cola, Cola *cola, opfn_t op);

// ================================
// Funciones para ABB
// ================================

// Para insertar un valor en un árbol.
bool insertarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
// Para buscar un valor en un árbol.
Nodo **buscarNodo(ABB, ABB *abb, const void *value_ptr, cmpfn_t cmp);
// Para eliminar un valor en un árbol.
bool eliminarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
// Para liberar la memoria de un árbol.
bool liberar(ABB, ABB *abb, freefn_t free_value);
// Para recorrer un árbol por PreOrder.
void forEachIn(ABB_PreOrder, ABB *abb, opfn_t op); // inicio izquierda derecha
// Para recorrer un árbol por InOrder.
void forEachIn(ABB_InOrder, ABB *abb, opfn_t op); // izquierda raiz derecha
// Para recorrer un árbol por PostOrder.
void forEachIn(ABB_PostOrder, ABB *abb, opfn_t op); // izquierda derecha raiz

#endif // NODOS_H