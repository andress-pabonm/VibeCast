#ifndef VIBECAST_NODOS_COLA_H
#define VIBECAST_NODOS_COLA_H 1

#include "fntypes.h"
#include <stddef.h>
#include <stdbool.h>

// ========================================
// Definición de tipos
// ========================================

typedef struct __Cola *Cola;

// ========================================
// API Pública
// ========================================

/**
 * @brief Crea una nueva cola vacía.
 * @return Un puntero a la nueva cola, o NULL si falla.
 */
Cola newCola();

/**
 * @brief Inserta un valor al final de la cola.
 * @param cola Cola en la que se insertará el valor.
 * @param value_ptr Puntero al valor a insertar.
 * @return true si se insertó correctamente, false si hubo un error.
 */
bool insertValueInCola(Cola cola, void *value_ptr);

/**
 * @brief Elimina el valor al frente de la cola.
 * @param cola Cola de la que se eliminará el valor.
 * @return Puntero al valor eliminado, o NULL si la cola estaba vacía o era inválida.
 */
void *deleteValueInCola(Cola cola);

/**
 * @brief Libera la memoria de toda la cola.
 * @param cola Cola que se va a destruir.
 * @param callback Función opcional a ejecutar por cada valor antes de liberar.
 * @param arg Argumento adicional que se pasará al callback.
 */
void destroyCola(Cola cola, operfn_t callback, void *arg);

#endif // VIBECAST_NODOS_COLA_H
