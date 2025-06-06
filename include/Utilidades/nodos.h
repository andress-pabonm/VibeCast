#ifndef NODOS_H
#define NODOS_H

#include "memmgr.h"
#include "utils.h"  // Asegura que "cast" esté definido

// ----------------------
// Listas simplemente ligadas
// ----------------------

typedef struct NodoLista {
    struct NodoLista *siguiente;
    void *valor;
} NodoLista;

typedef struct {
    NodoLista *inicio;
    // #ifdef LISTA_CON_LONGITUD
    // int longitud;
    // #endif
} Lista;

// ----------------------
// Pilas (stack)
// ----------------------

typedef struct NodoPila {
    struct NodoPila *debajo;
    void *valor;
} NodoPila;

typedef struct {
    NodoPila *cima;
} Pila;

// ----------------------
// Colas (queue)
// ----------------------

typedef struct NodoCola {
    struct NodoCola *siguiente;
    void *valor;
} NodoCola;

typedef struct {
    NodoCola *inicio;
    NodoCola *fin;
} Cola;

// ----------------------
// Árbol binario de búsqueda (ABB)
// ----------------------

typedef struct NodoABB {
    struct NodoABB *izq;
    struct NodoABB *der;
    void *valor;
} NodoABB;

typedef struct {
    NodoABB *raiz;
} ABB;

// ----------------------
// Utilidades y macros
// ----------------------

#define nuevoNodo(type, value_ptr) alloc(Nodo ## type, &cast(Nodo ## type, .valor = (value_ptr)))

#define insertarNodo(type, ...) insertarNodo ## type(__VA_ARGS__)
#define buscarNodo(type, ...) buscarNodo ## type(__VA_ARGS__)
#define eliminarNodo(type, ...) eliminarNodo ## type(__VA_ARGS__)
#define liberar(type, ...) liberar ## type(__VA_ARGS__)

// ----------------------
// Declaraciones de funciones
// ----------------------

// Lista
void insertarNodoLista(Lista *, NodoLista *, cmpfn_t);
NodoLista *buscarNodoLista(Lista *, void *, cmpfn_t);
void eliminarNodoLista(Lista *, NodoLista *);
void liberarLista(Lista *);

// Pila
void insertarNodoPila(Pila *, NodoPila *);
void *eliminarNodoPila(Pila *);
void liberarPila(Pila *);

// Cola
void insertarNodoCola(Cola *, NodoCola *);
void *eliminarNodoCola(Cola *);
void liberarCola(Cola *);

// Árbol binario de búsqueda
void insertarNodoABB(ABB *, NodoABB *, cmpfn_t);
NodoABB *buscarNodoABB(ABB *, void *, cmpfn_t);
void eliminarNodoABB(ABB *, NodoABB *, cmpfn_t);
void liberarABB(ABB *);

#endif // NODOS_H
