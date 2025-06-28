#ifndef VIBECAST_NODOS_PILA_H
#define VIBECAST_NODOS_PILA_H 1

#include "fntypes.h"
#include <stdbool.h>

// ========================================
// Definición de tipos
// ========================================

typedef struct __Pila *Pila;

// ========================================
// API Pública
// ========================================

/**
 * @brief Crea una nueva pila vacía.
 * @return Un puntero a la nueva pila, o NULL si falla.
 */
Pila newPila();

/**
 * @brief Inserta un valor en la cima de la pila.
 * @param pila Pila sobre la que se insertará el valor.
 * @param value_ptr Puntero al valor a insertar.
 * @return true si se insertó correctamente, false en caso de error.
 */
bool insertValueInPila(Pila pila, void *value_ptr);

/**
 * @brief Elimina el valor en la cima de la pila.
 * @param pila Pila de la que se eliminará el valor.
 * @return Puntero al valor eliminado, o NULL si la pila estaba vacía o era inválida.
 */
void *deleteValueInPila(Pila pila);

/**
 * @brief Libera la memoria de toda la pila.
 * @param pila Pila que se va a destruir.
 * @param callback Función opcional a ejecutar por cada valor antes de liberar.
 * @param arg Argumento adicional que se pasará al callback.
 */
void destroyPila(Pila pila, operfn_t callback, void *arg);

#endif // VIBECAST_NODOS_PILA_H
