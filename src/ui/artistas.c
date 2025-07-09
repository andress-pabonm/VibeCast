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
    if (!usuario->artista)
    {
        send_message("Error: El usuario no es un artista\n");
        return false;
    }

    const char *nombreAlbum = argv[0];

    if (searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre))
    {
        send_message("Error: El álbum '%s' ya existe\n", nombreAlbum);
        return false;
    }

    Album *album = newAlbum();
    album->nombre = asprintf(nombreAlbum);
    album->artista = usuario->artista;

    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());
    album->fechaCreacion = asprintf(fechaActual);

    insertValueInLista(usuario->artista->albumes, album);
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
    const int duracion = atoi(argv[2]);

    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        send_message("Error: Álbum no encontrado\n");
        return false;
    }

    if (searchValueInLista(album->canciones, nombreCancion, cmpCancionConNombre))
    {
        send_message("Error: La canción ya existe en este álbum\n");
        return false;
    }

    Cancion *cancion = newCancion();
    cancion->nombre = asprintf(nombreCancion);
    cancion->duracion = duracion;

    insertValueInLista(album->canciones, cancion);
    return true;
}

json_object *album_to_json(Album *album)
{
    json_object *jobj = json_object_new_object();
    json_object_object_add(jobj, "id", json_object_new_int(album->id));
    json_object_object_add(jobj, "name", json_object_new_string(album->nombre));
    json_object_object_add(jobj, "year", json_object_new_string(album->fechaCreacion));
    json_object_object_add(jobj, "genre", json_object_new_string("General"));

    json_object *songsArray = json_object_new_array();
    forEachInLista(album->canciones, ^(void *val, size_t idx, va_list _) {
      Cancion *cancion = val;
      json_object *songObj = json_object_new_object();
      json_object_object_add(songObj, "id", json_object_new_int(cancion->id));
      json_object_object_add(songObj, "title", json_object_new_string(cancion->nombre));
      json_object_object_add(songObj, "artist", json_object_new_string(album->artista->nombre));

      // Convertir duración de segundos a formato MM:SS
      char durationStr[10];
      snprintf(durationStr, sizeof(durationStr), "%d:%02d", cancion->duracion / 60, cancion->duracion % 60);
      json_object_object_add(songObj, "duration", json_object_new_string(durationStr));

      json_object_array_add(songsArray, songObj);
      return FOREACH_CONTINUE;
    },
                   NULL);

    json_object_object_add(jobj, "songs", songsArray);
    return jobj;
}

message_handler(obtener_albumes_artista)
{
    init_data_json();

    if (!usuario->artista)
    {
        VibeCast_SendText(id, HTTP_OK, "", "El usuario no es artista", STATE_FAILURE);
        return;
    }

    json_object *albumsArray = json_object_new_array();
    forEachInLista(usuario->artista->albumes, ^(void *val, size_t idx, va_list _) {
      Album *album = val;
      json_object *albumObj = album_to_json(album);
      json_object_array_add(albumsArray, albumObj);
      return FOREACH_CONTINUE;
    },
                   NULL);

    const char *jsonStr = json_object_to_json_string(albumsArray);
    VibeCast_SendText(id, HTTP_OK, jsonStr, "", STATE_SUCCESS);
    json_object_put(albumsArray);
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