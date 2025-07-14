#include <ui/interfaces.h>

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(ObtenerInfoArtista);
static new_operfn(getAlbumJSON);

static interfaz(CrearArtista);
static new_cmpfn(cmpArtistaConNombre);

static interfaz(EliminarArtista);

static interfaz(CrearAlbum);
static new_cmpfn(cmpAlbumConNombre);

static interfaz(EliminarAlbum);
// static new_cmpfn(cmpAlbumConId);

static interfaz(ActualizarAlbum);

static interfaz(CrearCancion);
static new_cmpfn(cmpCancionConNombre);
static select_handler(obteneridCancion);

static interfaz(EliminarCancion);

static interfaz(ActualizarCancion);

/* ======================================================== */
// BLOQUE: obtener_info_artista — Cargar artista en perfil
/* ======================================================== */

message_handler(obtener_info_artista)
{
    // Validar que el artista está creado
    if (!usuario->artista)
    {
        VibeCast_SendText(id, HTTP_OK, "", "El usuario no es artista", STATE_FAILURE);
        return;
    }

    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_ObtenerInfoArtista(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

static interfaz(ObtenerInfoArtista)
{
    json_object *response = arg;
    json_object *albumsArray = json_object_new_array();

    forEachInLista(usuario->artista->albumes, getAlbumJSON, albumsArray);

    json_object_object_add(response, "albums", albumsArray);

    send_message("Información de artista cargada.");
    return true;
}

static new_operfn(getAlbumJSON)
{
    Album *album = val;
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "id", json_object_new_int(album->id));
    json_object_object_add(jobj, "name", json_object_new_string(album->nombre));
    json_object_object_add(jobj, "year", json_object_new_string(album->fechaCreacion));

    json_object *songsArray = json_object_new_array();

    forEachInLista(album->canciones, getSongJSON, songsArray);

    json_object_object_add(jobj, "songs", songsArray);

    json_object_array_add(arg, jobj);

    return FOREACH_CONTINUE;
}

/* ============================================== */
// BLOQUE: crear_artista — Crea artista en perfil
/* ============================================== */

message_handler(crear_artista)
{
    init_data_json();

    const char *nombreArtista = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {nombreArtista};
    char **msg = arg;

    bool success = VibeCast_CrearArtista(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    puts(*msg);

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(CrearArtista)
{
    const char *nombreArtista = argv[0];

    Artista *artista = searchValueInABB(artistas, nombreArtista, cmpArtistaConNombre);
    if (artista)
    {
        send_message("El artista '%s' ya existe", nombreArtista);
        return false;
    }

    // char *values = asprintf(stringify("%d", "%s"), usuario->id, nombreArtista);
    // bool ok = nuevo_registro("Artistas", "id_usuario, nombre", values, NULL);
    // freem(values);

    // if (!ok)
    // {
    //     send_message("No fue posible crear el artista.");
    //     return false;
    // }

    artista = newArtista();
    usuario->artista = artista;
    artista->nombre = asprintf(nombreArtista);
    artista->usuario = usuario;

    insertValueInABB(artistas, artista);

    send_message("Artista creado.");
    return true;
}

static new_cmpfn(cmpArtistaConNombre)
{
    const Artista *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

/* ===================================================== */
// BLOQUE: eliminar_artista — Eliminar artista en perfil
/* ===================================================== */

message_handler(eliminar_artista)
{
    char **msg = arg;

    bool success = VibeCast_EliminarArtista(NULL, 0, NULL, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

static interfaz(EliminarArtista)
{
    bool deleteable = true;
    forEachInLista(usuario->artista->albumes, checkAlbums, &deleteable);

    if (!deleteable)
    {
        send_message("No es posible eliminar el perfil de artista porque alguna canción está añadida a alguna playlist.");
        return false;
    }

    destroyLista(usuario->artista->albumes, deleteAlbum, NULL);

    return true;
}

static new_operfn(checkAlbums)
{
    bool *deleteable = arg;
    Album *album = val;
    forEachInLista(album->canciones, checkSongs, deleteable);

    return *deleteable ? FOREACH_CONTINUE : FOREACH_BREAK;
}

static new_operfn(checkSongs)
{
    bool *deleteable = arg;
    Cancion *cancion = val;
    *deleteable = (!cancion->popularidad);

    return *deleteable ? FOREACH_CONTINUE : FOREACH_BREAK;
}

static new_operfn(deleteAlbum)
{
    Album *album = val;
    destroyLista(album->canciones, deleteSong, NULL);

    return FOREACH_CONTINUE;
}

static new_operfn(deleteSong)
{
    Cancion *cancion = val;

    char *condition = asprintf("id = %d", cancion->id);
    bool ok = eliminar_registros("Canciones", condition, NULL);
    freem(condition);

    destroyCancion(cancion);

    return FOREACH_CONTINUE;
}


/* ============================================ */
// BLOQUE: crear_album — Crear album de artista
/* ============================================ */

message_handler(crear_album)
{
    init_data_json();

    const char *nombreAlbum = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {nombreAlbum};
    char **msg = arg;

    bool success = VibeCast_CrearAlbum(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(CrearAlbum)
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

    send_message("Álbum creado.");
    return true;
}

static new_cmpfn(cmpAlbumConNombre)
{
    const Album *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

/* ================================================== */
// BLOQUE: eliminar_album — Eliminar album de artista
/* ================================================== */

message_handler(eliminar_album)
{
    char **msg = arg;

    bool success = VibeCast_EliminarAlbum(NULL, 0, NULL, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

static interfaz(EliminarAlbum)
{
    const char *nombreAlbum = argv[0];

    // Validar que el artista exista
    if (!usuario->artista)
    {
        send_message("Error: El usuario no es un artista\n");
        return false;
    }

    Lista albumes = usuario->artista->albumes;
    Album *album = searchValueInLista(albumes, nombreAlbum, cmpAlbumConNombre);

    // Validar que exista el álbum
    if (!album)
    {
        send_message("Error: El álbum '%s' no existe\n", nombreAlbum);
        return false;
    }

    if (getListaLength(album->canciones) > 0)
    {
        send_message("No es posible eliminar el álbum porque contiene canciones populares.");
        return false;
    }

    // Eliminar el álbum de la lista
    deleteValueInLista(albumes, album, cmpAlbumConNombre);

    destroyAlbum(album);

    send_message("Álbum eliminado.");
    return true;
}

/* ===================================================== */
// BLOQUE: actualizar_album — Actualiza album de artista
/* ===================================================== */

message_handler(actualizar_album)
{
    init_data_json();

    const char *nombreAlbumant = get_string(get_array_idx(data, 0));
    const char *nombreAlbumnew = get_string(get_array_idx(data, 1));

    int argc = 2;
    const char *argv[] = {nombreAlbumant, nombreAlbumnew};
    char **msg = arg;

    bool success = VibeCast_ActualizarAlbum(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(ActualizarAlbum)
{
    const char *nombreAlbumant = argv[0];
    const char *nombreAlbumnew = argv[1];

    // Validar que el artista exista
    if (!usuario->artista)
    {
        send_message("Error: El usuario no es un artista.");
        return false;
    }

    Lista albumes = usuario->artista->albumes;
    Album *album = searchValueInLista(albumes, nombreAlbumant, cmpAlbumConNombre);

    // Validar que exista el álbum
    if (!album)
    {
        send_message("Error: El álbum '%s' no existe.", nombreAlbumant);
        return false;
    }

    if (!strcmp(album->nombre, nombreAlbumnew))
    {
        send_message("No se actualizó el nombre del álbum porque es el mismo.");
        return false;
    }
    
    if(searchValueInLista(albumes, nombreAlbumnew, cmpAlbumConNombre))
    {
        send_message("Error: Ya existe un álbum con el nombre '%s'.", nombreAlbumnew);
        return false;
    }

    // Actualizar el nombre del álbum
    char *values = asprintf(stringify(nombre = "%s"), nombreAlbumnew);
    char *condition = asprintf("id = %d", album->id);
    bool ok = actualizar_registros("Albumes", values, condition, NULL);
    freem(values);
    freem(condition);
    if (!ok)
    {
        send_message("No fue posible actualizar el álbum.");
        return false;
    }

    freem(album->nombre);
    album->nombre = asprintf(nombreAlbumnew);

    send_message("Álbum actualizado.");
    return true;
}

/* ================================================= */
// BLOQUE: crear_cancion — Crear cancion en un album
/* ================================================= */

message_handler(crear_cancion)
{
    init_data_json();

    const char *nombreAlbum = get_string(get_array_idx(data, 0));
    const char *nombreCancion = get_string(get_array_idx(data, 1));
    const char *genero = get_string(get_array_idx(data, 2));
    const char *duracion = get_string(get_array_idx(data, 3));
    const char *url = get_string(get_array_idx(data, 4));

    int argc = 5;
    const char *argv[] = {nombreAlbum, nombreCancion, genero, duracion, url};
    char **msg = arg;

    bool success = VibeCast_CrearCancion(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(CrearCancion)
{
    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());

    const char *nombreAlbum = argv[0];
    const char *nombre = argv[1];
    const char *genero = argv[2];
    int duracion = atoi(argv[3]);
    const char *url = argv[4];

    Lista albumes = usuario->artista->albumes;
    Album *album = searchValueInLista(albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        send_message("El álbum no existe.");
        return false;
    }

    // Verificar si la canción ya existe en el álbum
    Cancion *cancion = searchValueInLista(album->canciones, nombre, cmpCancionConNombre);
    if (cancion)
    {
        send_message("Una cancion con ese nombre ya existe en el album\n");
        return false;
    }

    if (!getListaLength(album->canciones))
    {
        if (getListaLength(albumes) == 1)
        {
            char *values = asprintf(stringify("%d", "%s"), usuario->id, usuario->artista->nombre);
            bool ok = nuevo_registro("Artistas", "id_usuario, nombre", values, NULL);
            freem(values);

            if (!ok)
            {
                send_message("No fue posible guardar la información del artista en la base de datos.");
                return false;
            }
        }

        char *values = asprintf(stringify("%d", "%s", "%s"), usuario->id, album->nombre, album->fechaCreacion);
        bool ok = nuevo_registro("Albumes", "id_artista, nombre, fecha_creacion", values, NULL);
        freem(values);

        if (!ok)
        {
            send_message("No fue posible guardar la información del álbum en la base de datos.");
            return false;
        }
    }

    char *values = asprintf(
        stringify("%d", "%s", "%s", "%s", "%d", "%s"),
        album->id,
        nombre,
        genero,
        fechaActual,
        duracion,
        url);
    bool ok = nuevo_registro("Canciones", "id_album, nombre, gener, fecha_publicacion, duracion, url", values, NULL);
    freem(values);

    if (!ok)
    {
        send_message("No fue posible guardar la información de la canción en la base de datos.");
        return false;
    }

    obtener_registros("Canciones ORDER BY id DESC LIMIT 1", "*", NULL, cargarCancionesPorAlbum, album, NULL);

    return true;
}

static new_cmpfn(cmpCancionConNombre)
{
    const Cancion *c = val_1;
    const char *n = val_2;

    return strcmp(c->nombre, n);
}

/* ====================================================== */
// BLOQUE: eliminar_cancion — Elimina cancion en un album
/* ====================================================== */

message_handler(eliminar_cancion)
{
    init_data_json();

    const char *id_album = get_string(get_array_idx(data, 0));
    const char *id_cancion = get_string(get_array_idx(data, 1));

    int argc = 2;
    const char *argv[] = {id_album, id_cancion};
    char **msg = arg;

    bool success = VibeCast_EliminarCancion(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

interfaz(EliminarCancion)
{
    int id_album = atoi(argv[0]);
    int id_cancion = atoi(argv[1]);

    Lista albumes = usuario->artista->albumes;
    Album *album = searchValueInLista(albumes, &id_album, cmpAlbumConId);

    if (!album)
    {
        send_message("Álbum no encontrado.");
        return false;
    }

    Cancion *cancion = deleteValueInLista(album->canciones, &id_cancion, cmpCancionConId);

    if (!cancion)
    {
        send_message("Canción no encontrada.");
        return false;
    }

    if (cancion->popularidad)
    {
        insertValueInLista(album->canciones, cancion);
        send_message("No es posible eliminar la canción porque está agregada en alguna playlist.");
        return false;
    }

    char *condition = asprintf("id = %d", cancion->id);
    bool ok = eliminar_registros("Canciones", condition, NULL);
    freem(condition);

    if (!ok)
    {
        insertValueInLista(album->canciones, cancion);
        send_message("No fue posible eliminar la canción.");
        return false;
    }

    deleteValueInLista(canciones, &id_cancion, cmpCancionConId);

    destroyCancion(cancion);

    send_message("Canción eliminada.");
    return true;
}

static new_cmpfn(cmpAlbumConId)
{
    const Album *a = val_1;
    const int *id = val_2;

    return a->id - *id;
}

/* ====================================================== */
// BLOQUE: actualizar_cancion — Actualiza la información de una cancion en un album
/* ====================================================== */

message_handler(actualizar_cancion)
{
    init_data_json();
    const char *nombreAlbum = get_string(get_array_idx(data, 0));
    const char *id_cancion = get_string(get_array_idx(data, 1));
    const char *nuevoNombre = get_string(get_array_idx(data, 2));
    const char *nuevoGenero = get_string(get_array_idx(data, 3));
    const char *nuevaDuracion = get_string(get_array_idx(data, 4));
    const char *nuevaUrl = get_string(get_array_idx(data, 5));
    int argc = 6;

    const char *argv[] = {nombreAlbum, id_cancion, nuevoNombre, nuevoGenero, nuevaDuracion, nuevaUrl};

    char **msg = arg;

    bool success = VibeCast_ActualizarCancion(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));


    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(ActualizarCancion)
{
    // Asumiré que me envía el nombre del álbum y el id de la canción
    const char *nombreAlbum = argv[0];
    int id_cancion = atoi(argv[1]);
    const char *nuevoNombre = argv[2];
    const char *nuevoGenero = argv[3];
    int nuevaDuracion = atoi(argv[4]);
    const char *nuevaUrl = argv[5];

    // Busco el álbum
    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
    if (!album)
    {
        send_message("Álbum no encontrado.");
        return false;
    }

    // Luego busco la canción por id
    Cancion *cancion = searchValueInLista(album->canciones, &id_cancion, cmpCancionConId);
    if (!cancion)
    {
        send_message("No se encuentra la canción en el álbum.");
        return false;
    }

    //Actualizar el nombre, genero, duración y url
    
    return true;
}

static custom_interface(ActualizarNombreCancion, Cancion *cancion, const char *nuevoNombre)
{
    if (!cancion || !nuevoNombre || !*nuevoNombre)
    {
        send_message("Datos inválidos para actualizar la canción.");
        return false;
    }

    // Verificar si el nuevo nombre ya existe en el álbum
    if (searchValueInLista(cancion->album->canciones, nuevoNombre, cmpCancionConNombre))
    {
        send_message("Ya existe una canción con ese nombre en el álbum.");
        return false;
    }

    freem(cancion->nombre);
    cancion->nombre = asprintf(nuevoNombre);

    // Aquí se podría agregar código para actualizar la base de datos si es necesario

    send_message("Canción actualizada correctamente.");
    return true;
}

static custom_interface(ActualizarGeneroCancion, Cancion *cancion, const char *nuevoGenero)
{
    if (!cancion || !nuevoGenero || !*nuevoGenero)
    {
        send_message("Datos inválidos para actualizar el género de la canción.");
        return false;
    }

    freem(cancion->genero);
    cancion->genero = asprintf(nuevoGenero);

    // Aquí se podría agregar código para actualizar la base de datos si es necesario

    send_message("Género de la canción actualizado correctamente.");
    return true;
}

static custom_interface(ActualizarDuracionCancion, Cancion *cancion, int nuevaDuracion)
{
    if (!cancion || nuevaDuracion <= 0)
    {
        send_message("Datos inválidos para actualizar la duración de la canción.");
        return false;
    }

    cancion->duracion = nuevaDuracion;

    // Aquí se podría agregar código para actualizar la base de datos si es necesario

    send_message("Duración de la canción actualizada correctamente.");
    return true;
}

static custom_interface(ActualizarUrlCancion, Cancion *cancion, const char *nuevaUrl)
{
    if (!cancion || !nuevaUrl || !*nuevaUrl)
    {
        send_message("Datos inválidos para actualizar la URL de la canción.");
        return false;
    }

    freem(cancion->url);
    cancion->url = asprintf(nuevaUrl);

    // Aquí se podría agregar código para actualizar la base de datos si es necesario

    send_message("URL de la canción actualizada correctamente.");
    return true;
}
