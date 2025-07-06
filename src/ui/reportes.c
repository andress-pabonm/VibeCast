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

new_cmpfn(cmpPopularidadArtista)
{
    const PopularidadArtista *pa = val_1;
    const char *n = val_2;
    return strcmp(pa->artista, n);
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

    while (reproducciontemp)
    {
        // Recorremos la pila de reproducciones del usuario
        artistaTemp = searchValueInLista(
            listaArtistas,
            reproducciontemp->cancion->album->artista,
            cmpPopularidadArtista);

        if (artistaTemp) // Si el artista si esta en la lista de artistas
        {
            artistaTemp->numCancionesGuardadas++;
        }
        else
        {
            artistaTemp = alloc(PopularidadArtista, NULL);
            artistaTemp->artista = reproducciontemp->cancion->album->artista->nombre;
            artistaTemp->numCancionesGuardadas = 1;

            insertValueInLista(listaArtistas, artistaTemp); // Obtenemos el artista de la cancion
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

new_cmpfn(cmpArtistasPorPopularidad)
{
    const PopularidadArtista *pa = *(const PopularidadArtista **)val_1;
    const PopularidadArtista *pb = *(const PopularidadArtista **)val_2;

    return FOREACH_CONTINUE;
}

new_operfn(listar_anuncios)
{
    Usuario *u = val;
    FILE *archivo = arg;

    // Solo se imprime si el usuario está en plan FREE
    if (u->plan == PLAN_FREEMIUM)
        fprintf(archivo, "- %s: %d anuncios\n", u->username, u->historial.cantidadAnuncios);

    return FOREACH_CONTINUE;
}

new_operfn(listar_tiempo)
{
    Usuario *u = val;    // Puntero hacia el usuario actual desde el nodo del ABBS
    FILE *archivo = arg; //'arg' es el archivo donde se redactan los datos.
    fprintf(archivo, "- %s: %d segundos\n", u->username, u->historial.tiempoEscuchado);

    return FOREACH_CONTINUE; // Indica que el recorrido debe continuar con el siguiente usuario.
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

    fprintf(archivo, "Top 5 Canciones más reproducidas:\n\n");

    for (int i = 0; i < longitud && i < 5; i++)
    {
        fprintf(archivo, "%d) %d reproducciones\n", i + 1, reproducciones[i]);
    }

    fclose(archivo);
}

void generarTop3Artistas()
{
    Lista listaArtistas = newLista(NULL); // Crear una nueva lista para los artistas
    ObtenerHistorial(listaArtistas, 0, usuario);
    int longitud = getListaLength(listaArtistas);

    if (longitud == 0)
    {
        printf("No hay astistas disponibles\n");

        return;
    }

    PopularidadArtista **pArtistas = malloc_cpy(longitud * sizeof(PopularidadArtista *), 0); // vector en el heap

    for (int i = 0; i < longitud; i++)
    {
        pArtistas[i] = getValueInLista(listaArtistas, i); // Obtenemos los artistas de la lista
    }

    qsort(pArtistas, longitud, sizeof(PopularidadArtista *), cmpArtistasPorPopularidad); // Buffer, tamaño del buffer, tamaño de cada elemento, funcion de comparacion

    FILE *archivo = newFile("top3_artistas.txt", NULL);

    fprintf(archivo, "Top 3 artistas mas preferidos:\n");

    for (int i = 0; i < longitud && i < 3; i++)
    {
        fprintf(archivo, "%d) %s - %d canciones guardadas\n", i + 1, pArtistas[i]->artista, pArtistas[i]->numCancionesGuardadas);
    }

    fclose(archivo);
    freem(pArtistas);
}

// Funcion que genera un archivo .txt con tiempo total de reproduccion del usuario
void generarTiempoTotalReproduccion()
{
    // Crea un nuevo archivo de salida, si falla se detiene
    FILE *archivo = newFile("Tiempo_total_reproduccion.txt", NULL);

    if (!archivo)
        return;

    fprintf(archivo, "Tiempo total de reproduccion por usuario:\n\n");

    // Recorre todo el ABB de usuarios y llama a `listar_tiempo` con cada uno
    //  Pasa `archivo` como argumento para que cada usuario lo use
    forEachInABB(usuarios, listar_tiempo, NULL);

    fclose(archivo);
}

void generarCantidadAnunciosEscuchados()
{
    FILE *archivo = newFile("Anuncios_escuchados.txt", NULL);
    if (!archivo)
        return;

    fprintf(archivo, "Cantidad de anuncios escuchados por usuario (FREE):\n\n");

    // Recorre todo el ABB de usuarios y ejecuta el callback `listar_anuncios` con cada uno
    forEachInABB(usuarios, listar_anuncios, NULL);

    fclose(archivo);
}