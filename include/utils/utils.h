#ifndef UTILS_H
#define UTILS_H 1

#include <VibeCastConfig.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <sal.h>

/* ================ Tipos de funciones ================ */

/**
 * Tipo para funciones de comparación.
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
typedef int (*cmpfn_t)(const void *value_1_ptr, const void *value_2_ptr);

// Macro para facilitar declarar y definir funciones tipo (cmpfn_t)
#define new_cmp(name) int name(const void *value_ptr_1, const void *value_ptr_2)

/**
 * Tipo para funciones para operar sobre los valores en una lista, pila, cola o árbol.
 *
 * @param index: Índice del nodo en la lista. (Profundidad en caso de árboles).
 * @param value_ptr: Puntero al valor que contiene el nodo.
 *
 * @return true si debe continuar con el recorrido, caso contrario retorna false para detenerse.
 */
typedef bool (*opfn_t)(int index, void *value_ptr);

// Macro para facilitar declarar y definir funciones tipo (opfn_t)
#define new_op(name) bool name(int index, void *value_ptr)

/**
 * Tipo para funciones para liberar memoria.
 *
 * @param value_ptr: Puntero al valor del cual se liberará la memoria.
 * @return true si se ha podido liberar la memoria, caso contrario retorna false.
 */
typedef bool (*freefn_t)(void *value_ptr);

// Macro para facilitar declarar y definir funciones tipo (freefn_t)
#define new_free(name) bool name(void *value_ptr)

/**
 * Para asignar memoria en HEAP y copiar un valor en ella.
 * @param size: Cantidad de bytes a asignar. Si es 0, no asignará memoria.
 * @param value_ptr: Puntero al valor a copiar (puede ser NULL).
 * @return Puntero al espacio en HEAP asignado.
 * Ejemplo:
 * malloc_cpy(sizeof("Hola"), "Hola");
 * int num = 1;
 * int *ptr_num = malloc_cpy(sizeof(num), &num);
 */
void *malloc_cpy(int size, void *value_ptr);

/**
 * Para facilitar el uso de malloc_cpy() en ciertos casos.
 * @param type: Tipo de dato para asignar memoria.
 * @param value_ptr: Puntero al valor a copiar (puede ser NULL).
 * Ejemplo:
 * int num = 1;
 * int *ptr_num = alloc(int, &num);
 */
#define alloc(type, value_ptr) malloc_cpy(sizeof(type), value_ptr)

/**
 * Para asignar memoria en HEAP y copiar un string con un formato y parámetros dados.
 * @param fmt: Formato del string.
 * @param ...: Argumentos para el formato.
 * @return Puntero al espacio en HEAP.
 */
char *mprintf(_Printf_format_string_ const char *fmt, ...);

/**
 * Para convertir datos en un tipo dado.
 * @param type: Tipo de dato de salida.
 * @param ...: Valores a convertir.
 * Ejemplos:
 * int *ptr_num = &cast(int, 1);
 * cast(mi_struct, .campo1 = valor1, .campo2 = valor2, ...)
 */
#define cast(type, ...) ((type){__VA_ARGS__})

/**
 * Para convertir en string múltiples expresiones.
 * @param ...: Expresiones
 * Ejemplo:
 * stringify(hola, mundo) => "hola, mundo"
 */
#define stringify(...) #__VA_ARGS__

/**
 * Para unir dos expresiones.
 * @param expr1: Primera expresión
 * @param expr2: Segunda expresión
 * Ejemplo:
 * paste(hola, mundo) => holamundo
 */
#define paste(expr1, expr2) expr1##expr2

#endif // UTILS_H