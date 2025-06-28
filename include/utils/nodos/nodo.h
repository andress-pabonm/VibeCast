#ifndef VIBECAST_NODOS_NODO_H
#define VIBECAST_NODOS_NODO_H 1

typedef struct Nodo
{
    struct Nodo *l; // Izquierda
    struct Nodo *r; // Derecha
    void *v;        // Valor
} *Nodo;

// ================================
// Funciones para Nodo
// ================================

Nodo newNodo(void *value_ptr);
void destroyNodo(Nodo nodo);

#endif // VIBECAST_NODOS_NODO_H
