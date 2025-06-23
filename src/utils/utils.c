#include <utils/utils.h>
#include <stdlib.h> // Para malloc
#include <string.h> // Para memcpy
#include <stdarg.h> // Para va_list
#include <stdio.h>  // Para snprintf

void *malloc_cpy(int size, const void *src)
{
    void *dest = (size > 0) ? malloc(size) : NULL;
    return (dest && src) ? memcpy(dest, src, size) : dest;
}

char *mprintf(const char *fmt, ...)
{
    if (!fmt)
        return NULL; // El formato no puede ser NULL

    // Inicia una lista para los argumenotos variadicos
    va_list args;
    va_start(args, fmt);

    // Hacer una copia de los argumentos
    va_list args_copy;
    va_copy(args_copy, args);

    // Obtener la cantidad de caracteres para asignar memoria
    int len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy); // Cerrar args_copy

    // Comprobar si se necesita asignar memoria
    if (len < 1)
    {
        va_end(args); // Cerrar va_list
        return NULL;  // No se escribió nada
    }

    // Asignar memoria
    char *buffer = malloc(len + 1);

    if (buffer) // Ahora sí escribir el string
        vsnprintf(buffer, len + 1, fmt, args);
    va_end(args); // Cerrar va_list

    return buffer; // Retornar el resultado final
}
