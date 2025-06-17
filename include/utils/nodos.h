#ifndef NODOS_H
#define NODOS_H

#include <stdbool.h>

/* Tipo genérico para un nodo */
typedef struct Nodo
{
    struct Nodo *izq; // Nodo a la izquierda o anterior
    struct Nodo *der; // Nodo a la derecha o posterior
    void *value_ptr;  // Puntero al valor que guarda
} Nodo;

typedef struct
{
    Nodo *head;
} Lista;

typedef struct
{
    Nodo *top;
} Pila;

typedef struct
{
    Nodo *head;
    Nodo *tail;
} Cola;

typedef struct
{
    Nodo *root;
} ABB;

/* Declaración de funciones para estas estructuras */

/**
 * Tipo personalizado para las funciones de comparación.
 *
 * @param value_1_ptr: Puntero al primer valor.
 * @param value_2_ptr: Puntero al segundo valor.
 *
 * @return
 *
 * 0 si son iguales.
 *
 * Para orden ascendente:
 * - <0 si value_1 < value_2
 * - >0 si value_1 > value_2
 *
 * Para orden descendente:
 * - <0 si value_2 < value_1
 * - >0 si value_2 > value_1
 */
typedef int (*cmpfn_t)(void *value_1_ptr, void *value_2_ptr);

/**
 * Tipo personalizado para las funciones que evalúan un valor en un recorrido de una lista o árbol.
 *
 * @param index: Índice del nodo en la lista. (Profundidad en caso de árboles).
 * @param value_ptr: Puntero al valor que contiene el nodo.
 *
 * @return true si debe continuar con el recorrido, caso contrario retorna false para detenerse.
 */
typedef bool (*opfn_t)(int index, void *value_ptr);

/**
 * Tipo personalizado para las funciones de liberación de memoria.
 *
 * @param value_ptr: Puntero al valor del cual se liberará la memoria.
 * @return true si se ha podido liberar la memoria, caso contrario retorna false.
 */
typedef bool (*freefn_t)(void *value_ptr);

#define insertarNodo(type, ...) insertarNodo##type(__VA_ARGS__)
#define buscarNodo(type, ...) buscarNodo##type(__VA_ARGS__)
#define eliminarNodo(type, ...) eliminarNodo##type(__VA_ARGS__)
#define liberar(type, ...) liberar##type(__VA_ARGS__)
#define forEachIn(type, ...) forEachIn##type(__VA_ARGS__)

/* Lista */

bool insertarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
Nodo **buscarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
bool eliminarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
bool liberar(Lista, Lista *lista, freefn_t free_value);
void forEachIn(Lista, Lista *lista, opfn_t op);

/* Pila */

bool insertarNodo(Pila, Pila *pila, void *value_ptr);
void *eliminarNodo(Pila, Pila *pila);
bool liberar(Pila, Pila *pila, freefn_t free_value);
// void forEachIn(Pila, Pila *pila, opfn_t op);

/* Cola */

bool insertarNodo(Cola, Cola *cola, void *value_ptr);
void *eliminarNodo(Cola, Cola *cola);
bool liberar(Cola, Cola *cola, freefn_t free_value);
// void forEachIn(Cola, Cola *cola, opfn_t op);

/* ABB */

bool insertarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
Nodo **buscarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
bool eliminarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
bool liberar(ABB, ABB *abb, freefn_t free_value);
void forEachIn(ABB_PreOrder, ABB *abb, opfn_t op); //inicio izquierda derecha
void forEachIn(ABB_InOrder, ABB *abb, opfn_t op); //izquierda raiz derecha
void forEachIn(ABB_PostOrder, ABB *abb, opfn_t op); //izquierda derecha raiz

#endif // NODOS_H