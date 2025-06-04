#ifndef MEMMGR_H
#define MEMMGR_H

#include <stdlib.h>

/**
 * Allocates memory and copies the content of a given value.
 * @param size Number of bytes to allocate.
 * @param src Pointer to the value to copy.
 * @return Pointer to the newly allocated memory, or NULL on failure.
 */
void *mem_alloc_copy(size_t size, const void *src);

// Macro to simplify usage: alloc(int, &x)
#define alloc(type, value_ptr) mem_alloc_copy(sizeof(type), (value_ptr))

#endif // MEMMGR_H
