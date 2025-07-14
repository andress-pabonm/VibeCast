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

static interfaz(ActualizarAlbum);

static interfaz(CrearCancion);
static new_cmpfn(cmpCancionConNombre);
static select_handler(obteneridCancion);

static interfaz(EliminarCancion);

static interfaz(ActualizarCancion);

/* ======================================================== */
// BLOQUE: obtener_info_artista — Cargar artistas en perfil
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
    json_object *response = json_object_new_object();
    json_object *albumsArray = json_object_new_array();

    forEachInLista(usuario->artista->albumes, getAlbumJSON, albumsArray);

    json_object_object_add(response, "albums", albumsArray);

    VibeCast_SendJSON(arg, HTTP_OK, response, "Álbumes cargados", STATE_SUCCESS);

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

    const char *argv[] = {nombreArtista};
    int argc = sizeof(argv) / sizeof(*argv);

    char **msg = arg;
    bool success = VibeCast_CrearArtista(usuario, argc, argv, msg);

    VibeCast_SendText(id, HTTP_OK, *msg, success ? "Artista creado" : "Fallo al crear artista", STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

static interfaz(CrearArtista)
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
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_EliminarArtista(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

static interfaz(EliminarArtista)
{
    // Implmentar logica
    return true;
}

/* ============================================ */
// BLOQUE: crear_album — Crear album de artista
/* ============================================ */

message_handler(crear_album)
{
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_CrearAlbum(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
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
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_EliminarAlbum(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

static interfaz(EliminarAlbum)
{
    // Aplicar logica
    return true;
}

/* ===================================================== */
// BLOQUE: actualizar_album — Actualiza album de artista
/* ===================================================== */

message_handler(actualizar_album)
{
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_ActualizarAlbum(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

static interfaz(ActualizarAlbum)
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

/* ================================================= */
// BLOQUE: crear_cancion — Crear cancion en un album
/* ================================================= */

message_handler(crear_cancion)
{
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_CrearCancion(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
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

    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre);
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

    // Instancio la estructura Cancion
    cancion = newCancion();

    // Configurar propiedades de la canción
    cancion->album = album;
    cancion->nombre = asprintf(nombre);
    cancion->genero = asprintf(genero);
    cancion->fechaPublicacion = asprintf(fechaActual);
    cancion->duracion = duracion;
    cancion->url = asprintf(url);

    // Insertar en las estructuras de datos
    insertValueInLista(canciones, cancion);
    insertValueInLista(album->canciones, cancion);

    // Registrar en base de datos
    char *datos = asprintf(stringify("%d", "%s", "%s", "%s", "%s"), album->id, cancion->nombre, cancion->genero, cancion->fechaPublicacion, cancion->duracion, cancion->url);

    nuevo_registro("Canciones", "id_album, nombre, genero, fecha_publicacion, duracion, url", datos, NULL);

    // Obtener ID asignado por la base de datos
    obtener_registros("Canciones ORDER BY id DESC LIMIT 1", "id", NULL, obteneridCancion, &cancion->id, NULL);

    return true;
}

static new_cmpfn(cmpCancionConNombre)
{
    const Cancion *c = val_1;
    const char *n = val_2;
    return strcmp(c->nombre, n);
}

static select_handler(obteneridCancion)
{
    sscanf(argv[0], "%d", cast(int *, arg)); // Asigna el ID de la canción al puntero proporcionado
    return 0;
}

/* ====================================================== */
// BLOQUE: eliminar_cancion — Elimina cancion en un album
/* ====================================================== */

message_handler(eliminar_cancion)
{
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_EliminarCancion(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

interfaz(EliminarCancion)
{
    int id = atoi(argv[0]); // Creo que es esta función
    // Buscar la canción en la lista global
    Cancion *cancion = searchValueInLista(canciones, &id, cmpCancionConId);

    if (cancion == NULL)
    {
        send_message("La canción con el id %d no existe\n", id);
        return false;
    }

    // Verifica si hay al menos una playlist que contiene la canción
    // if ()
    // {
    //     // Logica para encontrar al menos una playlist que contenga la canción
    // }

    // Buscar el álbum al que pertenece la canción
    Album *album = searchValueInLista(usuario->artista->albumes, &(cancion->album->nombre), cmpAlbumConNombre);
    if (!album)
    {
        send_message("No se encontró el álbum asociado a la canción\n");
        return false;
    }

    // Eliminar de las estructuras de datos
    deleteValueInLista(album->canciones, &id, cmpCancionConId);
    deleteValueInLista(canciones, &id, cmpCancionConId);

    // Liberar memoria de los strings
    free(cancion->fechaPublicacion);
    free(cancion->genero);
    free(cancion->nombre);
    free(cancion->url);

    // Eliminar de la base de datos APLCAR LA LOGICA NECESARIA
    // char *condicion = asprintf("ID = %d", id);
    // eliminar_registros("Canciones", condicion, NULL);
    // free(condicion);

    // Liberar la estructura de la canción
    free(cancion);

    send_message("Canción eliminada exitosamente\n");

    return true;
}

/* ====================================================== */
// BLOQUE: actualizar_cancion — Elimina cancion en un album
/* ====================================================== */

message_handler(actualizar_cancion)
{
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_ActualizarCancion(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

static interfaz(ActualizarCancion)
{
    // Asumiré que me envía el nombre del álbum y el id de la canción
    const char *nombreAlbum = argv[0];
    int id_cancion = atoi(argv[1]);

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

    /* Aquí se actualizarían los datos de la canción */

    return true;
}