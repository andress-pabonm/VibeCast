#include <ui/interfaces.h>

new_cmpfn(cmpIntDesc)
{
    const int *n1 = val_1;
    const int *n2 = val_2;

    return *n1 - *n2; // Orden descendente
}

new_operfn(obtener_reproducciones)
{
    Cancion *cancion = val;
    int *reproducciones = arg;

<<<<<<< HEAD
    longitud = getListaLength(canciones);

    
=======
    if (cancion)
        reproducciones[idx] = cancion->reproducciones; // Guardamos la popularidad en el vector

    return FOREACH_CONTINUE; // Continuar iterando
}

void generarTop5Canciones()
{
    int longitud = getListaLength(canciones);
    if (longitud == 0)
    {
        printf("No hay canciones disponibles para generar el top 5\n");

        return;
    }

    int *reproducciones = malloc_cpy(longitud * sizeof(int), 0); // vector en el heap

    forEachInLista(canciones, obtener_reproducciones, reproducciones);

    qsort(reproducciones, longitud, sizeof(int), cmpIntDesc); // Buffer, tamaño del buffer, tamaño de cada elemento, funcion de comparacion

    FILE *archivo = newFile("top5_canciones.txt", NULL);

    fprintf(archivo, "Top 5 Canciones más reproducidas:\n");

    for (int i = 0; i < longitud && i < 5; i++)
    {
        fprintf(archivo, "%d) %d reproducciones\n", i + 1, reproducciones[i]);
    }
>>>>>>> bcae8e050b3dba89279147e3a884056172d82bdb
}