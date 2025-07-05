#include <ui/interfaces.h>

typedef struct PopularidadArtista
{
    const char *artista;       // Artista
    int numCancionesGuardadas; // Cantidad de reproducciones
} PopularidadArtista;

new_cmpfn(cmpIntDesc)
{
    const int *n1 = val_1;
    const int *n2 = val_2;

    return *n1 - *n2; // Orden descendente
}

static new_cmpfn(cmpArtistaConNombre)
{
    const Artista *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

new_operfn(obtener_reproducciones)
{
    Cancion *cancion = val;
    int *reproducciones = arg;

    if (cancion)
        reproducciones[idx] = cancion->reproducciones; // Guardamos la popularidad en el vector

    return FOREACH_CONTINUE; // Continuar iterando
}

new_operfn(ObtenerHistorial)
{
    Usuario *usuario = val;    // val=void *val, que es un puntero a Usuario que estan en la lista de amigos
    Lista listaArtistas = arg; // argumento para la funcion de reccorrer la lista de amigos
    Pila historial = usuario->historial.reproducciones;
    Pila tempHistorial = newPila();
    // Obtenemos la pila de reproducciones del historial del usuario
    PopularidadArtista *artistaTemp;
    Reproduccion *reproducciontemp;

    reproducciontemp = deleteValueInPila(historial);

    while (reproducciontemp != NULL)
    {
        // Recorremos la pila de reproducciones del usuario
        artistaTemp->artista = reproducciontemp->cancion->album->artista;
        artistaTemp->artista = searchValueInABB(artistas, artistaTemp->artista, cmpArtistaConNombre); // Buscamos el artista en la lista de artistas

        insertValueInLista(listaArtistas, artistaTemp); // Obtenemos el artista de la cancion

        if (!artistas) // Si el artista no esta en la lista de artistas
        {
        }
        else
        {
        }

        insertValueInPila(tempHistorial, reproducciontemp); // Insertamos la reproduccion en la pila temporal

        reproducciontemp = deleteValueInPila(historial); // Obtenemos la siguiente reproduccion
    }

    reproducciontemp = deleteValueInPila(tempHistorial); // Obtenemos la ultima reproduccion

    while (reproducciontemp != NULL)
    {
        insertValueInPila(historial, reproducciontemp);
        reproducciontemp = deleteValueInPila(tempHistorial); // Insertamos las reproducciones de la pila temporal en la pila de reproducciones del historial
    }

    destroyPila(tempHistorial, NULL, NULL); // Destruimos la pila temporal

    return FOREACH_CONTINUE; // Continuar recorriendo la lista de amigos
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

    fclose(archivo);
}

void generarTop3Artistas()
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

    fclose(archivo);
}

Lista recomendarCanciones()
{
    Lista ListaCancionesRecomendadas = newLista(NULL); // Crear una nueva lista para las canciones recomendadas
    forEachInLista(usuario->amigos, ObtenerHistorial, ListaCancionesRecomendadas);
    // recorrer la lista de amigos y llamar a la funcion recomendar_canciones
    return ListaCancionesRecomendadas; // Retornar la lista de canciones recomendadas
}
