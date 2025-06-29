#ifndef VIBECAST_LOG_H
#define VIBECAST_LOG_H 1

#include <stddef.h>

/**
 * Función para añadir un mensaje a un archivo.
 *
 * @param filename: Nombre del archivo en el que se va a añadir el mensaje.
 * @param message: Mensaje a añadir al archivo.
 */
void log(const char *filename, const char *message);

/**
 * Para asignar memoria en HEAP y copiar un string con un formato y parámetros dados.
 * @param fmt: Formato del string.
 * @param ...: Argumentos para el formato.
 * @return Puntero al espacio en HEAP.
 */
char *asprintf(const char *fmt, ...);

#endif // VIBECAST_LOG_H