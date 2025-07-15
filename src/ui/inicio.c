#include <ui/interfaces.h>

/* ================================================================ */
// DEFINICIÓN DE ESTRUCTURAS INTERNAS
/* ================================================================ */

typedef struct
{
    Pila tmpHistorial;
    Lista songs_array;
} getReprSong_arg_t;

typedef struct PopularidadArtista
{
    const Artista *artista; // Artista
    int popularidad;        // Cantidad de veces que se ha escuchado al artista
} PopularidadArtista;

/* ================================================================ */
// DECLARACIÓN DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(ObtenerCanciones);

static interfaz(ObtenerRecomendaciones);
static new_operfn(getFriendReprSongs);
static new_operfn(getReprSong);

static interfaz(GenerarReporte);

static custom_interface(GenerarTop5Canciones);
static new_operfn(obtenerReproducciones);
static new_cmpfn(cmpRepr);

static custom_interface(GenerarTop3Artistas);
static new_operfn(obtenerArtistasHistorial);
static new_cmpfn(cmpPopularidadArtista);
static new_operfn(copiarLista);
static new_cmpfn(cmpArtistasPorPopularidad);

static custom_interface(TiempoTotalReproduccion);

static custom_interface(CantidadAnuncios);

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
    forEachInLista(usuario->amigos, getFriendReprSongs, recomendaciones);

    destroyLista(recomendaciones, getSongJSON, arg);

    if (json_object_array_length(arg))
        send_message("Recomendaciones cargadas.");
    else
        send_message("No hay recomendaciones disponibles.");

    return true;
}

static new_operfn(getFriendReprSongs)
{
    Usuario *amigo = val;

    getReprSong_arg_t wrapper_arg = {
        .tmpHistorial = newPila(),
        .songs_array = arg,
    };

    destroyPila(amigo->historial.reproducciones, getReprSong, &wrapper_arg);
    amigo->historial.reproducciones = newPila();
    destroyPila(wrapper_arg.tmpHistorial, rehacerHistorial, amigo->historial.reproducciones);

    return FOREACH_CONTINUE;
}

static new_operfn(getReprSong)
{
    getReprSong_arg_t *wrapper_arg = arg;
    Reproduccion *repr = val;

    insertValueInPila(wrapper_arg->tmpHistorial, repr);

    Cancion *cancion = searchValueInLista(wrapper_arg->songs_array, &repr->cancion->id, cmpCancionConId);

    if (!cancion)
    {
        cancion = searchValueInLista(canciones, &repr->cancion->id, cmpCancionConId);
        insertValueInLista(wrapper_arg->songs_array, cancion);
    }

    return FOREACH_CONTINUE;
}

new_operfn(rehacerHistorial)
{
    insertValueInPila(arg, val);
    return FOREACH_CONTINUE;
}

/* ======================================================= */
// BLOQUE: generar_reporte — Estadísticas de la aplicación
/* ======================================================= */

message_handler(generar_reporte)
{
    json_object *estadisticas = json_object_new_object();

    char **msg = arg;
    bool success = VibeCast_GenerarReporte(estadisticas, 0, NULL, msg);

    VibeCast_SendJSON(id, HTTP_OK, estadisticas, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
    json_object_put(estadisticas);
}

static interfaz(GenerarReporte)
{

    json_object *top_songs = json_object_new_array();
    VibeCast_GenerarTop5Canciones(top_songs, msg);
    json_object_object_add(arg, "topCanciones", top_songs);

    json_object *top_artists = json_object_new_array();
    VibeCast_GenerarTop3Artistas(top_artists, msg);
    json_object_object_add(arg, "topArtistas", top_artists);

    VibeCast_TiempoTotalReproduccion(NULL, msg);
    json_object_object_add(arg, "tiempoEscuchado", json_object_new_int(usuario->historial.tiempoEscuchado));

    VibeCast_CantidadAnuncios(NULL, msg);
    json_object_object_add(arg, "cantidadAnuncios", json_object_new_int(usuario->historial.cantidadAnuncios));

    send_message("Ahi esta sus reportes");

    return true;
}

static custom_interface(GenerarTop5Canciones)
{
    int longitud = getListaLength(canciones);

    if (longitud == 0)
    {
        send_message("No hay canciones disponibles para generar el top 5\n");
        return false;
    }

    Cancion **reproducciones = malloc_cpy(longitud * sizeof(Cancion *), 0); // vector en el heap

    forEachInLista(canciones, obtenerReproducciones, reproducciones);

    qsort(reproducciones, longitud, sizeof(Cancion *), cmpRepr); // Buffer, tamaño del buffer, tamaño de cada elemento, funcion de comparacion

    FILE *archivo = newFile("Top 5 canciones.txt", NULL);

    fprintf(archivo, "Top 5 Canciones más reproducidas:\n\n");

    for (int i = 0; i < longitud && i < 5; i++)
    {
        Cancion *cancion = reproducciones[i];

        // 1) Loba - 5 reproducciones
        fprintf(archivo, "%d) %s - %d reproducciones\n", i + 1, cancion->nombre, cancion->reproducciones);

        json_object *jobj = json_object_new_object();

        json_object_object_add(jobj, "nombreCancion", json_object_new_string(cancion->nombre));
        json_object_object_add(jobj, "reproducciones", json_object_new_int(cancion->reproducciones));

        json_object_array_add(arg, jobj);
    }

    fclose(archivo);
    freem(reproducciones);

    return true;
}

static new_operfn(obtenerReproducciones)
{
    Cancion **reproducciones = arg;

    reproducciones[idx] = val;

    return FOREACH_CONTINUE; // Continuar iterando
}

static new_cmpfn(cmpRepr)
{
    const Cancion *n1 = val_1;
    const Cancion *n2 = val_2;

    return n2->reproducciones - n1->reproducciones; // Orden descendente
}

static custom_interface(GenerarTop3Artistas)
{
    Lista listaArtistas = newLista(NULL); // Crear una nueva lista para los artistas
    getReprSong_arg_t wrapper_arg = {
        .tmpHistorial = newPila(),
        .songs_array = listaArtistas,
    };

    destroyPila(usuario->historial.reproducciones, obtenerArtistasHistorial, &wrapper_arg);

    usuario->historial.reproducciones = newPila();
    destroyPila(wrapper_arg.tmpHistorial, rehacerHistorial, usuario->historial.reproducciones);

    int longitud = getListaLength(listaArtistas);

    if (longitud == 0)
    {
        send_message("No hay astistas disponibles\n");
        return false;
    }

    PopularidadArtista **pArtistas = malloc_cpy(longitud * sizeof(PopularidadArtista *), 0); // vector en el heap
    forEachInLista(listaArtistas, copiarLista, pArtistas);

    qsort(pArtistas, longitud, sizeof(PopularidadArtista *), cmpArtistasPorPopularidad); // Buffer, tamaño del buffer, tamaño de cada elemento, funcion de comparacion

    FILE *archivo = newFile("Top 3 artistas.txt", NULL);

    fprintf(archivo, "Top 3 artistas mas preferidos:\n");

    for (int i = 0; i < longitud && i < 3; i++)
    {
        PopularidadArtista *pArtista = pArtistas[i];

        // 1) Shakira - 5 canciones reproducidas
        fprintf(archivo, "%d) %s - %d canciones reproducidas\n", i + 1, pArtista->artista->nombre, pArtista->popularidad);

        json_object *jobj = json_object_new_object();

        json_object_object_add(jobj, "nombreArtista", json_object_new_string(pArtista->artista->nombre));
        json_object_object_add(jobj, "reproducciones", json_object_new_int(pArtista->popularidad));

        json_object_array_add(arg, jobj);

        freem(pArtista);
    }

    fclose(archivo);
    destroyLista(listaArtistas, NULL, NULL);
    freem(pArtistas);

    return true;
}

static new_operfn(obtenerArtistasHistorial)
{
    getReprSong_arg_t *wrapper_arg = arg;
    Reproduccion *reproducciontemp = val;
    PopularidadArtista *artistaTemp;

    // Recorremos la pila de reproducciones del usuario
    artistaTemp = searchValueInLista(wrapper_arg->songs_array, reproducciontemp->cancion->album->artista, cmpPopularidadArtista);

    if (artistaTemp) // Si el artista si esta en la lista de artistas
        artistaTemp->popularidad++;
    else
    {
        artistaTemp = alloc(PopularidadArtista, NULL);
        artistaTemp->artista = reproducciontemp->cancion->album->artista;
        artistaTemp->popularidad = 1;

        insertValueInLista(wrapper_arg->songs_array, artistaTemp); // Obtenemos el artista de la cancion
    }

    insertValueInPila(wrapper_arg->tmpHistorial, reproducciontemp); // Insertamos la reproduccion en la pila temporal

    return FOREACH_CONTINUE; // Continuar recorriendo la lista de amigos
}

static new_cmpfn(cmpPopularidadArtista)
{
    const PopularidadArtista *pa = val_1;

    return cmpArtistaConNombre(pa->artista, val_2);
}

static new_operfn(copiarLista)
{
    void **lista = arg;
    lista[idx] = val;

    return FOREACH_CONTINUE;
}

static new_cmpfn(cmpArtistasPorPopularidad)
{
    const PopularidadArtista *pa = val_1;
    const PopularidadArtista *pb = val_2;

    return pb->popularidad - pa->popularidad;
}

static custom_interface(TiempoTotalReproduccion)
{
    // Crea un nuevo archivo de salida, si falla se detiene
    FILE *archivo = newFile("Tiempo total de reproduccion.txt", NULL);

    fprintf(archivo, "Tiempo total de reproduccion:\n\n");

    fprintf(archivo, "%d", usuario->historial.tiempoEscuchado);

    fclose(archivo);

    return true;
}

static custom_interface(CantidadAnuncios)
{
    FILE *archivo = newFile("Anuncios escuchados.txt", NULL);

    fprintf(archivo, "Cantidad de anuncios escuchados solo FREE:\n\n");

    fprintf(archivo, "%d", usuario->historial.cantidadAnuncios);

    fclose(archivo);

    return true;
}