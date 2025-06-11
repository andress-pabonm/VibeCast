#ifndef NODOS_H
#define NODOS_H

#include <stdbool.h>
#include <utils/utils.h>

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

#define insertarNodo(type, ...) insertarNodo##type(__VA_ARGS__)
#define buscarNodo(type, ...) buscarNodo##type(__VA_ARGS__)
#define eliminarNodo(type, ...) eliminarNodo##type(__VA_ARGS__)
#define liberar(type, ...) liberar##type(__VA_ARGS__)

/* Lista */

bool insertarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
Nodo **buscarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
bool eliminarNodo(Lista, Lista *lista, void *value_ptr, cmpfn_t cmp);
bool liberar(Lista, Lista *lista, freefn_t free_value);

/* Pila */

bool insertarNodo(Pila, Pila *pila, void *value_ptr);
void *eliminarNodo(Pila, Pila *pila);
bool liberar(Pila, Pila *pila, freefn_t free_value);

/* Cola */

bool insertarNodo(Cola, Cola *cola, void *value_ptr);
void *eliminarNodo(Cola, Cola *cola);
bool liberar(Cola, Cola *cola, freefn_t free_value);

/* ABB */

bool insertarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
Nodo **buscarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
bool eliminarNodo(ABB, ABB *abb, void *value_ptr, cmpfn_t cmp);
bool liberar(ABB, ABB *abb, freefn_t free_value);

#endif // NODOS_H