#ifndef VIBECAST_MACROS_H
#define VIBECAST_MACROS_H 1

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

#endif // VIBECAST_MACROS_H