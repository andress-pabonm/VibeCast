#ifndef VIBECAST_MEMMGR_H
#define VIBECAST_MEMMGR_H 1

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
 * Función para liberar memoria.
 */
void freem(void *ptr);

#endif // VIBECAST_MEMMGR_H