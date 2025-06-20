#include <utils/utils.h>
#include <stdlib.h> // Para malloc
#include <string.h> // Para memcpy
#include <stdarg.h> // Para va_list
#include <stdio.h>  // Para snprintf

void *malloc_cpy(size_t size, const void *src)
{
    void *dest = size ? malloc(size) : NULL;
    return (dest && src) ? memcpy(dest, src, size) : dest;
}

char *mprintf(const char *fmt, ...)
{
    char buffer[1024] = {0};

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, sizeof(buffer), fmt, args);

    return malloc_cpy(strlen(buffer) + 1, buffer);
}
