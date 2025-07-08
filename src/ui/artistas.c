#include <ui/interfaces.h>

static new_cmpfn(cmpArtistaConNombre)
{
    const Artista *a = val_1; // Puntero a estructura Artista
    const char *n = val_2;    // Puntero a nombre (string) a comparar

    return strcmp(a->nombre, n); // Compara el nombre del artista con el nombre dado
}

static new_cmpfn(cmpAlbumConNombre)
{
    const Album *a = val_1; // Puntero a estructura Album
    const char *n = val_2;  // Puntero a nombre (string) a comparar

    return strcmp(a->nombre, n); // Compara el nombre del álbum con el nombre dado
}

static new_operfn(mostrar_canciones)
{
    Cancion *cancion = val;                    // Puntero a la canción actual
    printf("%d): %s\n", idx, cancion->nombre); // Muestra el índice y el nombre de la canción

    return FOREACH_CONTINUE; // Continúa el bucle
}

interfaz(CrearArtista)
{
    const char *nombreArtista = argv[0];

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

// Ejemplo de creación de canción con la macro (interfaz) =============================================
interfaz(CrearCancion)
{
    const char *nombre = argv[0];

    Cancion **cancion = arg;
    *cancion = newCancion();

    send_message("Canción creada");

    return true;
}

message_handler(crear_cancion)
{
    // Inicializa la información que llega desde la interfaz gráfica
    init_data_json();

    const char *nombreCancion = get_string(get_array_idx(data, 0)); // Parecido a: nombreCancion = data[0]

    // Crea un variable Cancion
    Cancion *cancion = NULL;

    // Empaquetar los argumentos
    const char *argv[] = {
        nombreCancion,
    };

    // Cantidad de argumentos
    int argc = sizeof(argv) / sizeof(*argv);

    char **msg = arg;

    // Llamar a la función para crear canción.
    bool success = VibeCast_CrearCancion(&cancion, 1, argv, msg);

    if (success)
    {
        /* Canción creada */
        VibeCast_SendText(id, HTTP_OK, "", *msg, STATE_SUCCESS);

        puts(*msg); // Imprimir el mensaje en consola
        freem(*msg);
        *msg = NULL;
    }
    else
    {
        /* Canción no creada */
        VibeCast_SendText(id, HTTP_OK, "", "", STATE_FAILURE);
    }
}
// Fin ejemplo =============================================

interfaz(CrearAlbum)
{
    const char *nombreAlbum = argv[0];

    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());

    // Verificar si el álbum ya existe
    if (searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre))
    {
        send_message("Error: El álbum '%s' ya existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
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

interfaz(AgregarCancionAlbum)
{
    // const char *nombreCancion = argv[0];
    const char *nombreAlbum = argv[1];

    // Buscar el álbum
    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        send_message("Error: El álbum '%s' no existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return false;
    }

    if (getListaLength(album->canciones) == 0)
    {
        // registro de album en la base de datos , hay que que agregar la funcion de guardar album en la base de datos
        // return false;
    }

    // Crea una nueva canción en canciones.c
    Cancion *cancion = crearCancion(album, nombreCancion, genero, duracion, url);

    // Insertar en la lista de canciones del álbum
    insertValueInLista(album->canciones, cancion);

    return true;
}

json_object *album_to_json(Album *album)
{
    json_object *jobj = json_object_new_object();

    return jobj;
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