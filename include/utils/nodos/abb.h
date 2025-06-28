#ifndef VIBECAST_NODOS_ABB_H
#define VIBECAST_NODOS_ABB_H 1

#include "fntypes.h"

// ========================================
// Definición de tipo ABB
// ========================================

typedef struct __ABB *ABB;

// ========================================
// API Pública - Creación
// ========================================

/**
 * Crea un nuevo Árbol Binario de Búsqueda (ABB).
 * @param cmp Función de comparación obligatoria.
 * @return Un ABB válido o NULL si cmp es NULL.
 */
ABB newABB(cmpfn_t cmp);

// ========================================
// API Pública - Inserción
// ========================================

/**
 * Inserta un valor en el ABB respetando el orden.
 * @return Profundidad de inserción o INSERT_FAILED si no fue posible.
 */
int insertValueInABB(ABB abb, void *value_ptr);

// ========================================
// API Pública - Búsqueda
// ========================================

/**
 * Busca un valor en el ABB usando la función de comparación dada.
 * @return Un puntero al valor encontrado o NULL si no existe.
 */
void *searchValueInABB(ABB abb, void *value_ptr, cmpfn_t cmp);

// ========================================
// API Pública - Eliminación
// ========================================

/**
 * Elimina un valor del ABB si existe.
 * @return El valor eliminado o NULL si no fue encontrado.
 */
void *deleteValueInABB(ABB abb, void *value_ptr, cmpfn_t cmp);

// ========================================
// API Pública - Recorridos
// ========================================

/**
 * Recorre el ABB en Pre-Orden, ejecutando un callback para cada valor.
 */
void forEachInABB_PreOrder(ABB abb, operfn_t callback, void *arg);

/**
 * Recorre el ABB en In-Orden, ejecutando un callback para cada valor.
 */
void forEachInABB_InOrder(ABB abb, operfn_t callback, void *arg);

/**
 * Recorre el ABB en Post-Orden, ejecutando un callback para cada valor.
 */
void forEachInABB_PostOrder(ABB abb, operfn_t callback, void *arg);

// ========================================
// API Pública - Destrucción
// ========================================

/**
 * Libera toda la memoria usada por el ABB.
 * Si se proporciona un callback, este se invocará por cada valor.
 */
void destroyABB(ABB abb, operfn_t callback, void *arg);

#endif // VIBECAST_NODOS_ABB_H
