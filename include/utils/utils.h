#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include <stdlib.h> // Para system()

// ---------------------------
// Macros útiles
// ---------------------------

// Limpia la entrada estándar después de un scanf
#define flush()               \
    while (getchar() != '\n') \
    continue

// scanf seguido de flush para evitar basura en el buffer
int scan(const char *fmt, ...);

// Limpia la pantalla
#ifdef _WIN32
#define clear() system("cls")
#else
#define clear() system("clear")
#endif

// Limpia stdin y luego limpia la pantalla
#define waitAndClear() \
    do                 \
    {                  \
        flush();       \
        clear();       \
    } while (0)

// ---------------------------
// Macro de casting compuesto
// ---------------------------
// Permite crear literales de tipo compuesto: cast(Type, campo1 = val1, campo2 = val2, ...)
// Ejemplo: cast(Punto, .x = 5, .y = 10)
#define cast(type, ...) ((type){__VA_ARGS__})

typedef int (*cmpfn_t)(void *, void *);

typedef bool (*freefn_t)(void *);

int menu(const char title[], int opcc, const char *opcv[]);

/**
 * Allocates memory and copies the content of a given value.
 * @param size Number of bytes to allocate.
 * @param src Pointer to the value to copy.
 * @return Pointer to the newly allocated memory, or NULL on failure.
 */
void *mem_alloc_copy(size_t size, const void *src);

// Macro to simplify usage: alloc(int, &x)
#define alloc(type, value_ptr) mem_alloc_copy(sizeof(type), (value_ptr))

// Macros para llamadas a funciones polimórficas por tipo
#define cmpfn(type, ...) cmp##type(__VA_ARGS__)
#define validar(type, ...) validar##type(__VA_ARGS__)

// Declaración de funciones de validación por tipo
// (Se implementan por separado en otro archivo, por ejemplo: validarInt, validarFloat, etc.)

#endif // UTILS_H
