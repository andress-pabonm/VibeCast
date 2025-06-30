#include <utils/log.h>
#include <utils/datetime.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void log_to_file(const char *filename, const char *message)
{
    if (filename && message)
    {
        FILE *file = fopen(filename, "a");

        datetime_buf_t dt_buf;
        getDateTime(dt_buf, now());
        fprintf(file, "%s %s\n", dt_buf, message);

        fclose(file);
    }
}

char *asprintf(const char *fmt, ...)
{
    if (!fmt)        // Si no hay un formato
        return NULL; // No hay nada que imprimir

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
    char *buffer =
        (len > 0)             // Si se escribió al menos un caracter
            ? malloc(len + 1) // Asigna memoria
            : NULL;

    if (buffer)            // Si se asignó memoria
        vsnprintf(buffer,  // Escribir en este búfer
                  len + 1, // De este tamaño
                  fmt,     // El formato indicado aquí
                  args);   // Aplicando estos argumentos

    va_end(args); // Cerrar va_list

    return buffer; // Retornar el resultado final
}
