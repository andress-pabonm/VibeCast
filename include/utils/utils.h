#ifndef UTILS_H
#define UTILS_H 1

#include <stddef.h> // Para size_t

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
void *malloc_cpy(size_t size, void *value_ptr);

/**
 * Para facilitar el uso de malloc_cpy() en ciertos casos.
 * @param type: Tipo de dato para asignar memoria.
 * @param value_ptr: Puntero al valor a copiar (puede ser NULL).
 * Ejemplo:
 * int num = 1;
 * int *ptr_num = alloc(int, &num);
 */
#define alloc(type, value_ptr) malloc_cpy()

/**
 * Para asignar memoria en HEAP y copiar un string con un formato y parámetros dados.
 * @param fmt: Formato del string.
 * @param ...: Argumentos para el formato.
 * @return Puntero al espacio en HEAP.
 */
char *mprintf(const char *fmt, ...);

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