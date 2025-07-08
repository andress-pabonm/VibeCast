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

interfaz(GenerarTop5Canciones)
{
    int longitud = getListaLength(canciones);

    if (longitud == 0)
    {
        send_message("No hay canciones disponibles para generar el top 5\n");
        return false;
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

    return true;
}

interfaz(GenerarTop3Artistas)
{
    Lista listaArtistas = newLista(NULL); // Crear una nueva lista para los artistas
    ObtenerHistorial(listaArtistas, 0, usuario);
    int longitud = getListaLength(listaArtistas);

    if (longitud == 0)
    {
        send_message("No hay astistas disponibles\n");
        return false;
    }

    PopularidadArtista **pArtistas = malloc_cpy(longitud * sizeof(PopularidadArtista *), 0); // vector en el heap

    for (int i = 0; i < longitud; i++)
    {
        pArtistas[i]->artista = getValueInLista(listaArtistas, i); // Obtenemos los artistas de la lista, da error y pues no se como solucionar xd
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

    return true;
}

// Funcion que genera un archivo .txt con tiempo total de reproduccion del usuario
interfaz(GenerarTiempoTotalReproduccion)
{
    // Crea un nuevo archivo de salida, si falla se detiene
    FILE *archivo = newFile("Tiempo_total_reproduccion.txt", NULL);

    if (!archivo)
        return false;

    fprintf(archivo, "Tiempo total de reproduccion por usuario:\n\n");

    // Recorre todo el ABB de usuarios y llama a `listar_tiempo` con cada uno
    //  Pasa `archivo` como argumento para que cada usuario lo use
    // forEachInABB(usuarios, listar_tiempo, NULL);

    // Aplica lo mismo que está en la función para la cantidad de anuncios.

    fprintf(archivo, "%d", usuario->historial.tiempoEscuchado);

    fclose(archivo);

    return true;
}

interfaz(GenerarCantidadAnunciosEscuchados)
{
    FILE *archivo = newFile("Anuncios_escuchados.txt", NULL);

    fprintf(archivo, "Cantidad de anuncios escuchados por usuario (FREE):\n\n");

    // Recorre todo el ABB de usuarios y ejecuta el callback `listar_anuncios` con cada uno
    // forEachInABB(usuarios, listar_anuncios, NULL);

    // Primero, este reporte es únicamente sobre el usuario activo,
    // es decir, no sobre todos los usuarios registrados. Entonces,
    // únicamente hay que acceder a la variable del historial:
    // usuario->historial.cantidadAnuncios;

    // Segundo, no existe la función forEachInABB como tal, sino
    // que tiene 3 variantes: preorden, inorden y postorden.
    // forEachInABB_PreOrder
    // forEachInABB_InOrder
    // forEachInABB_PostOrder

    // PERO AQUI FALTA MOSTRAR XDDD
    // Es cierto, aún no sé cómo mostrar los reportes,
    // pero ya veré qué es lo que pide en la interfaz
    // gráfica.

    // Entonces, esto va así
    fprintf(archivo, "%d", usuario->historial.cantidadAnuncios);

    fclose(archivo);

    return true;
}

message_handler(get_estadisticas)
{
}