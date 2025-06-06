#include <Utilidades/memmgr.h>
#include <string.h>

static inline void *mem_alloc_copy(size_t size, const void *src) {
    void *dest = size ? malloc(size) : NULL;
    return (dest && src) ? memcpy(dest, src, size) : dest;
}
