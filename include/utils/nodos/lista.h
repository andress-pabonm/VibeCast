/**
 * @file lista.h
 * @brief API para listas enlazadas genéricas en C.
 *
 * Esta biblioteca permite crear y manipular listas enlazadas simples
 * con punteros genéricos, soportando inserción ordenada, búsqueda,
 * eliminación, recorrido y destrucción.
 */

#ifndef VIBECAST_NODOS_LISTA_H
#define VIBECAST_NODOS_LISTA_H 1

#include "fntypes.h" // Define cmpfn_t y operfn_t
#include <stddef.h>

/**
 * @typedef Lista
 * @brief Tipo opaco que representa una lista enlazada.
 */
typedef struct __Lista *Lista;

/**
 * @brief Crea una nueva lista vacía.
 *
 * @param cmp Regla de ordenamiento de la lista.
 *
 * @return Lista nueva o NULL si hay error de memoria.
 */
Lista newLista(cmpfn_t cmp);

/**
 * @brief Inserta un valor en la lista.
 *
 * Si se proporciona una función de comparación, el valor se inserta
 * ordenadamente; de lo contrario, se inserta al final.
 *
 * @param lista Lista destino.
 * @param value_ptr Puntero al valor a insertar.
 * @return Índice del nuevo elemento, o INSERT_FAILED si falla.
 */
int insertValueInLista(Lista lista, void *value_ptr);

/**
 * @brief Busca un valor en la lista.
 *
 * @param lista Lista donde buscar.
 * @param value_ptr Valor a buscar.
 * @param cmp Función de comparación.
 * @return Puntero al valor encontrado, o NULL si no se encuentra.
 */
void *searchValueInLista(Lista lista, const void *value_ptr, cmpfn_t cmp); 

/**
 * @brief Elimina un valor de la lista.
 *
 * @param lista Lista donde eliminar.
 * @param value_ptr Valor a eliminar.
 * @param cmp Función de comparación.
 * @return Puntero al valor eliminado, o NULL si no se encuentra.
 */
void *deleteValueInLista(Lista lista, const void *value_ptr, cmpfn_t cmp);

/**
 * @brief Recorre todos los elementos de la lista en orden.
 *
 * @param lista Lista a recorrer.
 * @param callback Función que se ejecuta por cada valor.
 * @param arg Argumento adicional para el callback.
 */
void forEachInLista(Lista lista, operfn_t callback, void *arg);

/**
 * @brief Libera toda la memoria asociada a la lista.
 *
 * Si se proporciona un callback, se ejecuta por cada valor antes de liberarlo.
 *
 * @param lista Lista a destruir.
 * @param callback Callback opcional para liberar valores.
 * @param arg Argumento adicional para el callback.
 */
void destroyLista(Lista lista, operfn_t callback, void *arg);

#endif // VIBECAST_NODOS_LISTA_H
