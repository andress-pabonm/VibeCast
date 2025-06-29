#include <utils/memmgr.h>
#include <stdlib.h> // Para calloc
#include <string.h> // Para memcpy

void *malloc_cpy(int size, void *src)
{
    void *dest =
        (size > 0)         // Si el tamaño es mayor a 0
            ? malloc(size) // Intentar asignar memoria
            : NULL;
    return (src && dest)      // Si hay un origen y un destino
               ? memcpy(dest, // Copiar en el destino
                        src,  // El contenido del origen
                        size) // Que consiste en (size) bytes
               : dest;
}

void freem(void *ptr)
{
    free(ptr);
}
