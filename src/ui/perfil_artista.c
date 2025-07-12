#include <ui/interfaces.h>

static new_cmpfn(cmpArtistaConNombre)
{
    const Artista *a = val_1;
    const char *n = val_2;
    return strcmp(a->nombre, n);
}

static new_cmpfn(cmpAlbumConNombre)
{
    const Album *a = val_1;
    const char *n = val_2;
    return strcmp(a->nombre, n);
}

static new_cmpfn(cmpCancionConNombre)
{
    const Cancion *c = val_1;
    const char *n = val_2;
    return strcmp(c->nombre, n);
}

interfaz(CrearArtista)
{
    const char *nombreArtista = argv[0];

    if (searchValueInABB(artistas, nombreArtista, cmpArtistaConNombre))
    {
        send_message("Error: El artista '%s' ya existe\n", nombreArtista);
        return false;
    }

    Artista *artista = newArtista();
    usuario->artista = artista;
    artista->nombre = asprintf(nombreArtista);
    artista->usuario = usuario;

    insertValueInABB(artistas, artista);

    return true;
}

interfaz(CrearAlbum)
{
    // Validar que el artista exista
    if (!usuario->artista)
    {
        send_message("Error: El usuario no es un artista\n");
        return false;
    }

    const char *nombreAlbum = argv[0];

    Lista albumes = usuario->artista->albumes;
    Album *album = searchValueInLista(albumes, nombreAlbum, cmpAlbumConNombre);

    // Validar que no exista el álbum ya
    if (album)
    {
        send_message("Error: El álbum '%s' ya existe\n", nombreAlbum);
        return false;
    }

    // Crear nuevo álbum
    album = newAlbum();
    album->nombre = asprintf(nombreAlbum);
    album->artista = usuario->artista;

    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());
    album->fechaCreacion = asprintf(fechaActual);

    insertValueInLista(albumes, album);
    return true;
}

interfaz(AgregarCancionAlbum)
{
    if (!usuario->artista)
    {
        send_message("Error: El usuario no es un artista\n");
        return false;
    }

    const char *nombreAlbum = argv[0];
    const char *nombreCancion = argv[1];
    int duracion = atoi(argv[2]);

    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        send_message("Error: Álbum no encontrado\n");
        return false;
    }

    Cancion *cancion = searchValueInLista(album->canciones, nombreCancion, cmpCancionConNombre);

    if (cancion)
    {
        send_message("Error: La canción ya existe en este álbum\n");
        return false;
    }

    // Crear nueva canción
    cancion = newCancion();
    cancion->nombre = asprintf(nombreCancion);
    cancion->duracion = duracion;

    insertValueInLista(album->canciones, cancion);
    return true;
}

new_operfn(getAlbumJSON)
{
    Album *album = val;
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "id", json_object_new_int(album->id));
    json_object_object_add(jobj, "name", json_object_new_string(album->nombre));
    json_object_object_add(jobj, "year", json_object_new_string(album->fechaCreacion));

    json_object *songsArray = json_object_new_array();

    forEachInLista(
        album->canciones,
        getSongJSON,
        songsArray);

    json_object_object_add(jobj, "songs", songsArray);

    json_object_array_add(arg, jobj);
    return FOREACH_CONTINUE;
}

message_handler(get_artist_data)
{
    // Validar que el artista está creado
    if (!usuario->artista)
    {
        VibeCast_SendText(id, HTTP_OK, "", "El usuario no es artista", STATE_FAILURE);
        return;
    }

    json_object *response = json_object_new_object();
    json_object *albumsArray = json_object_new_array();

    forEachInLista(
        usuario->artista->albumes,
        getAlbumJSON,
        albumsArray);

    json_object_object_add(response, "albums", albumsArray);

    VibeCast_SendJSON(
        id,
        HTTP_OK,
        response,
        "Álbumes cargados",
        STATE_SUCCESS);

    puts(json_object_to_json_string(response));

    // Liberar memoria
    json_object_put(response);
}

/*
{
  "albums": [
    {
      "id": 1,
      "name": "Mi álbum",
      "year": "2025",
      "songs": [
        {
          "id": 101,
          "title": "Canción 1",
          "artist": "Nombre del artista",
          "duration": "3:12"
        }
      ]
    },
    // más álbumes...
  ]
}
 */

message_handler(obtener_info_artista)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(crear_artista)
{
    init_data_json();
    const char *nombreArtista = get_string(get_array_idx(data, 0));

    const char *argv[] = {nombreArtista};
    int argc = sizeof(argv) / sizeof(*argv);

    char **msg = arg;
    bool success = VibeCast_CrearArtista(usuario, argc, argv, msg);

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

message_handler(eliminar_artista)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(crear_album)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(eliminar_album)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(actualizar_album)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(crear_cancion)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(eliminar_cancion)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}

message_handler(actualizar_cancion)
{
    VibeCast_SendNull(id, HTTP_OK, "", STATE_SUCCESS);
}
