#include <Utilidades/utils.h>
#include <stdarg.h>
#include <stdio.h>

static inline int scan(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int r = vscanf(fmt, args);
    va_end(args);
    flush();
    return r;
}