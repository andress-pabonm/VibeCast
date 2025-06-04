#ifndef UTILS_H
#define UTILS_H

// #include <stdbool.h>   // Para los validadores
#include <stdio.h>     // Para scanf y getchar
#include <stdlib.h>    // Para system()
#include <stdarg.h>    // Para la función scan

// ---------------------------
// Macros útiles
// ---------------------------

// Limpia la entrada estándar después de un scanf
#define flush() while (getchar() != '\n') continue

// scanf seguido de flush para evitar basura en el buffer
static inline int scan(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = vscanf(fmt, args);
    va_end(args);
    flush();
    return r;
}


// Limpia la pantalla
#ifdef _WIN32
    #define clear() system("cls")
#else
    #define clear() system("clear")
#endif

// Limpia stdin y luego limpia la pantalla
#define waitAndClear() do { flush(); clear(); } while (0)

// ---------------------------
// Macro de casting compuesto
// ---------------------------
// Permite crear literales de tipo compuesto: cast(Type, campo1 = val1, campo2 = val2, ...)
// Ejemplo: cast(Punto, .x = 5, .y = 10)
#define cast(type, ...) ((type){__VA_ARGS__})

// ---------------------------
// Tipos y funciones genéricas
// ---------------------------

// Tipo para funciones de comparación tipo strcmp
typedef int (*cmpfn_t)(void *, void *);

// Macros para llamadas a funciones polimórficas por tipo
#define cmpfn(type, ...) cmp ## type (__VA_ARGS__)
#define validar(type, ...) validar ## type (__VA_ARGS__)

// Declaración de funciones de validación por tipo
// (Se implementan por separado en otro archivo, por ejemplo: validarInt, validarFloat, etc.)

#endif // UTILS_H
