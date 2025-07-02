#include <utils/filemgr.h>
#include <utils/log.h>
#include <stdlib.h>

FILE *newFile(const char *filename, char **errmsg)
{
    printf("Creando archivo: '%s'\n", filename);

    FILE *file = fopen(filename, "w+");
    if (!file && !(file = tmpfile()) && errmsg)
        *errmsg = asprintf("Error al crear el archivo");
    return file;
}

FILE *getFile(const char *filename, char **errmsg)
{
    printf("Abriendo archivo: '%s'\n", filename);

    FILE *file = fopen(filename, "r+");
    if (!file && errmsg)
        *errmsg = asprintf("Error al abrir el archivo: %s", filename);
    return file;
}

char *readFile(const char *filename, char **errmsg)
{
    printf("Leyendo archivo: '%s'\n", filename);

    FILE *file = getFile(filename, errmsg);
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    int len = ftell(file);

    if (len < 1)
    {
        fclose(file);
        if (errmsg)
            *errmsg = asprintf("Archivo vacio.");
        return NULL;
    }

    char *buf = malloc(len + 1);

    if (buf)
    {
        rewind(file);
        fread(buf, len, 1, file);
        buf[len] = '\0';
    }
    else if (errmsg)
    {
        *errmsg = asprintf("Error de memoria.");
    }

    fclose(file);

    return buf;
}