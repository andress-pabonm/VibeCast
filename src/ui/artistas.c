#include <ui/interfaces.h>

new_cmpfn(cmpArtistaConNombre)
{
    const Artista *a = val_1; // Puntero a estructura Artista
    const char *n = val_2;    // Puntero a nombre (string) a comparar

    return strcmp(a->nombre, n); // Compara el nombre del artista con el nombre dado
}

new_cmpfn(cmpAlbumConNombre)
{
    const Album *a = val_1; // Puntero a estructura Album
    const char *n = val_2;  // Puntero a nombre (string) a comparar

    return strcmp(a->nombre, n); // Compara el nombre del álbum con el nombre dado
}

new_operfn(mostrar_canciones)
{
    Cancion *cancion = val;                     // Puntero a la canción actual
    printf("[%d]: %s\n", idx, cancion->nombre); // Muestra el índice y el nombre de la canción

    return FOREACH_CONTINUE; // Continúa el bucle
}

bool crearNombreArtista(const char *nombreArtista)
{
    // Verificar si el artista ya existe
    if (searchValueInABB(artistas, nombreArtista, cmpArtistaConNombre))
    {
        printf("Error: El artista '%s' ya existe\n", nombreArtista);
        return false;
    }

    // Crea nueva estructura de artista
    Artista *artista = newArtista();

    usuario->artista = artista;
    artista->nombre = asprintf(nombreArtista);
    artista->usuario = usuario;

    // Insertar en el árbol de artistas
    insertValueInABB(artistas, artista);

    return true;
}

bool crearAlbum(const char *nombreAlbum)
{
    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());

    // Verificar si el álbum ya existe
    if (searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre))
    {
        printf("Error: El álbum '%s' ya existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return false;
    }

    // Crear nueva estructura de álbum
    Album *album = newAlbum();

    album->nombre = asprintf(nombreAlbum);
    album->artista = usuario->artista;
    album->fechaCreacion = asprintf(fechaActual);

    // Insertar en la lista de álbumes del artista
    insertValueInLista(usuario->artista->albumes, album);

    return true;
}

bool agregarCancionAlbum(const char *nombreAlbum, const char *nombreCancion, const char *genero, int duracion, const char *url)
{
    // Buscar el álbum
    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        printf("Error: El álbum '%s' no existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return false;
    }

    // Crear nueva canción en canciones.c
    Cancion *cancion = crearCancion(album, nombreCancion, genero, duracion, url);

    // Insertar en la lista de canciones del álbum
    insertValueInLista(album->canciones, cancion);

    return true;
}

void mostrarAlbum(const char *nombreAlbum)
{
    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        printf("Album no existente");
        return;
    }

    printf("Artista: %s\n", album->artista->nombre);
    printf("Fecha de lanzamiento: %s\n", album->fechaCreacion);
    printf("Album: %s\n", album->nombre);
    printf("Canciones: \n");

    // Mostrar todas las canciones del álbum
    forEachInLista(album->canciones, mostrar_canciones, NULL);
}

message_handler(crear_artista)
{
    init_data_json();

    const char *nombreArtista = get_string(get_array_idx(data, 0));
    const char *argv[] = {nombreArtista};
    int argc = sizeof(argv) / sizeof(*argv);

    char **msg = arg;
    bool success = VibeCast_crearArtista(usuario, argc, argv, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        success ? "Artista creado" : "Fallo al crear artista",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}