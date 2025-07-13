#include <ui/interfaces.h>

/* ================================================================ */
// DEFINICIÓN DE ESTRUCTURAS INTERNAS
/* ================================================================ */

typedef struct PopularidadArtista
{
    Artista *artista; // Artista
    int popularidad;  // Cantidad de veces que se ha escuchado al artista
} PopularidadArtista;

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(ObtenerCanciones);

static interfaz(ObtenerRecomendaciones);
static Lista cancionesRecomendadas();
static new_operfn(obtenerHistorialAmigos);

static interfaz(GenerarReporte);

/* ================================================================ */
// BLOQUE: obtener_canciones — Cargar canciones en la sección de inicio
/* ================================================================ */

message_handler(obtener_canciones)
{
    json_object *songs_array = json_object_new_array();
    char **msg = arg;

    bool success = VibeCast_ObtenerCanciones(songs_array, 0, NULL, msg);
    VibeCast_SendArray(id, HTTP_OK, songs_array, *msg, STATE_SUCCESS);

    freem(*msg);
    *msg = NULL;

    json_object_put(songs_array);
}

static interfaz(ObtenerCanciones)
{
    forEachInLista(canciones, getSongJSON, arg);

    if (json_object_array_length(arg))
        send_message("Canciones cargadas.");
    else
        send_message("No hay canciones disponibles.");

    return true;
}

/* ================================================================ */
// BLOQUE: obtener_recomendaciones — Recomendaciones para el usuario
/* ================================================================ */

message_handler(obtener_recomendaciones)
{
    json_object *array = json_object_new_array();
    char **msg = arg;

    bool success = VibeCast_ObtenerRecomendaciones(array, 0, NULL, msg);
    VibeCast_SendArray(id, HTTP_OK, array, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(array);
}

static interfaz(ObtenerRecomendaciones)
{
    Lista recomendaciones = newLista(NULL);
    forEachInLista(usuario->amigos, obtenerHistorialAmigos, recomendaciones);

    destroyLista(
        recomendaciones,
        getSongJSON,
        arg);

    if (json_object_array_length(arg))
        send_message("Recomendaciones cargadas.");
    else
        send_message("No hay recomendaciones disponibles.");

    return true;
}

static new_operfn(obtenerHistorialAmigos)
{
    Usuario *amigo = val;

    Pila historial = amigo->historial.reproducciones;
    Pila tmpHistorial = newPila();

    Reproduccion *repr = deleteValueInPila(historial);
    Cancion *cancion;

    while (repr)
    {
        cancion = searchValueInLista(arg, &repr->cancion->id, cmpCancionConId);

        if (!cancion)
        {
            cancion = searchValueInLista(canciones, &repr->cancion->id, cmpCancionConId);
            insertValueInLista(arg, cancion);
        }

        insertValueInPila(tmpHistorial, repr);
        repr = deleteValueInPila(historial);
    }

    destroyPila(tmpHistorial, rehacerHistorial, historial);

    return FOREACH_CONTINUE;
}

new_operfn(rehacerHistorial)
{
    insertValueInPila(arg, val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// BLOQUE: generar_reporte — Estadísticas de la aplicación
/* ================================================================ */

message_handler(generar_reporte)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

static inerfaz(GenerarReporte)
{
}

static new_cmpfn(cmpIntDesc)
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

static new_operfn(obtenerReproducciones)
{
    Cancion *cancion = val;
    int *reproducciones = arg;

    if (cancion)
        reproducciones[idx] = cancion->reproducciones; // Guardamos la popularidad en el vector

    return FOREACH_CONTINUE; // Continuar iterando
}

static new_cmpfn(cmpPopularidadArtista)
{
    const PopularidadArtista *pa = val_1;
    const char *n = val_2;

    return strcmp(pa->artista->nombre, n);
}

static new_operfn(ObtenerHistorial)
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
            artistaTemp->popularidad++;
        }
        else
        {
            artistaTemp = alloc(PopularidadArtista, NULL);
            artistaTemp->artista = searchValueInABB(artistas, reproducciontemp->cancion->album->artista->nombre, cmpArtistaConNombre);
            artistaTemp->popularidad = 1;

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

    forEachInLista(canciones, obtenerReproducciones, reproducciones);

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
        fprintf(archivo, "%d) %s - %d canciones guardadas\n", i + 1, pArtistas[i]->artista->nombre, pArtistas[i]->popularidad);
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