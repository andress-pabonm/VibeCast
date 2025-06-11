#include <utils/utils.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *mem_alloc_copy(size_t size, const void *src)
{
    void *dest = size ? malloc(size) : NULL;
    return (dest && src) ? memcpy(dest, src, size) : dest;
}

int scan(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int r = vscanf(fmt, args);
    va_end(args);
    flush();
    return r;
}

int menu(const char title[], int opcc, const char *opcv[])
{
    if (opcc < 1 || !opcv)
        return 0;

    if (title)
        puts(title);

    int opc, inputCount;

    for (opc = 0; opc < opcc; opc++)
        printf("%d. %s\n", opc + 1, opcv[opc]);

    do
    {
        printf("Elige una opción: ");
        inputCount = scan("%d", &opc);

        if (!inputCount)
            puts("Entrada no válida.");
    } while (!inputCount);

    return opc;
}