#include <ui/interfaces.h>

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(ObtenerPlaylists);
static new_operfn(getPlaylistJSON);
static new_operfn(getPlaylistSongsJSON);

static interfaz(CrearPlaylist);
static new_cmpfn(cmpPlaylistConNombre);

static interfaz(EliminarPlaylist);
static new_cmpfn(cmpPlaylistConId);

static interfaz(ActualizarPlaylist);

static interfaz(AgregarAPlaylist);

static interfaz(EliminarDePlaylist);

/* ================================================================ */
// BLOQUE: obtener_playlists — Cargar playlists en la sección de biblioteca
/* ================================================================ */

message_handler(obtener_playlists)
{
    json_object *response = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_ObtenerPlaylists(response, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, response, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    json_object_put(response);
}

static interfaz(ObtenerPlaylists)
{
    json_object *playlists_array = json_object_new_array();
    json_object *songs_array = json_object_new_object();

    forEachInLista(usuario->playlists, getPlaylistJSON, playlists_array);
    forEachInLista(usuario->playlists, getPlaylistSongsJSON, songs_array);

    json_object_object_add(arg, "playlists", playlists_array);
    json_object_object_add(arg, "playlistSongs", songs_array);

    send_message("Playlists cargadas.");
    return true;
}

static new_operfn(getPlaylistJSON)
{
    Playlist *playlist = val;
    json_object *playlist_json = json_object_new_object();

    json_object_object_add(playlist_json, "id", json_object_new_int(playlist->id));
    json_object_object_add(playlist_json, "name", json_object_new_string(playlist->nombre));
    json_object_object_add(playlist_json, "songCount", json_object_new_int(getListaLength(playlist->canciones)));

    json_object_array_add(arg, playlist_json);

    return FOREACH_CONTINUE;
}

static new_operfn(getPlaylistSongsJSON)
{
    Playlist *playlist = val;

    // Crea un arreglo para las canciones de la playlist
    json_object *songs_array = json_object_new_array();

    // Recorre la playlist
    forEachInLista(playlist->canciones, getSongJSON, songs_array);

    // Añadirlo al arreglo principal
    char *id_playlist = asprintf("%d", playlist->id);
    json_object_object_add(arg, id_playlist, songs_array);
    freem(id_playlist);

    return FOREACH_CONTINUE;
}

new_operfn(getSongJSON)
{
    Cancion *cancion = val;
    json_object *song_json = json_object_new_object();

    json_object_object_add(song_json, "id", json_object_new_int(cancion->id));
    json_object_object_add(song_json, "title", json_object_new_string(cancion->nombre));
    json_object_object_add(song_json, "artist", json_object_new_string(cancion->album->artista->nombre));
    json_object_object_add(song_json, "duration", json_object_new_int(cancion->duracion));
    json_object_object_add(song_json, "url", json_object_new_string(cancion->url));

    json_object_array_add(arg, song_json);

    return FOREACH_CONTINUE;
}

/* ================================================================ */
// BLOQUE: crear_playlist — Crear una nueva playlist
/* ================================================================ */

message_handler(crear_playlist)
{
    init_data_json();

    const char *nombrePlaylist = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {nombrePlaylist};
    char **msg = arg;

    bool success = VibeCast_CrearPlaylist(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(CrearPlaylist)
{
    const char *nombrePlaylist = argv[0];

    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre);
    if (playlist)
    {
        send_message("La playlist '%s' ya existe.", nombrePlaylist);
        return false;
    }

    char *values = asprintf(stringify("%d", "%s"), usuario->id, nombrePlaylist);
    nuevo_registro("Playlists", "id_usuario, nombre", values, NULL);
    freem(values);

    obtener_registros("Playlists ORDER BY id DESC LIMIT 1", "id, nombre", NULL, cargarPlaylistsPorUsuario, usuario->playlists, NULL);

    send_message("Playlist creada.");
    return true;
}

static new_cmpfn(cmpPlaylistConNombre)
{
    const Playlist *p = val_1;
    const char *n = val_2;

    return strcmp(p->nombre, n);
}

/* ================================================================ */
// BLOQUE: eliminar_playlist — Eliminar una playlist existente
/* ================================================================ */

message_handler(eliminar_playlist)
{
    init_data_json();

    const char *id_playlist = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {id_playlist};
    char **msg = arg;

    bool success = VibeCast_EliminarPlaylist(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(EliminarPlaylist)
{
    int id = atoi(argv[0]);

    Playlist *playlist = deleteValueInLista(usuario->playlists, &id, cmpPlaylistConId);
    if (!playlist)
    {
        send_message("Playlist no encontrada.");
        return false;
    }

    char *condition = asprintf("id = %d", id);
    eliminar_registros("Playlists", condition, NULL);

    send_message("Playlist eliminada.");
    return true;
}

static new_cmpfn(cmpPlaylistConId)
{
    const Playlist *p = val_1;
    const int *id = val_2;

    return p->id - *id;
}

/* ================================================================ */
// BLOQUE: actualizar_playlist — Actualizar datos de una playlist
/* ================================================================ */

message_handler(actualizar_playlist)
{
    init_data_json();

    const char *id_playlist = get_string(get_array_idx(data, 0));
    const char *nombrePlaylist = get_string(get_array_idx(data, 1));

    int argc = 2;
    const char *argv[] = {id_playlist, nombrePlaylist};
    char **msg = arg;

    bool success = VibeCast_ActualizarPlaylist(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    end_data_json();
}

static interfaz(ActualizarPlaylist)
{
    int id = atoi(argv[0]);
    const char *nombrePlaylist = argv[1];

    Playlist *playlist = searchValueInLista(usuario->playlists, &id, cmpPlaylistConId);
    if (!playlist)
    {
        send_message("Playlist no encontrada.");
        return false;
    }

    if (!strcmp(playlist->nombre, nombrePlaylist))
    {
        send_message("No se actualizó el nombre de la playlist.");
        return false;
    }

    freem(playlist->nombre);
    playlist->nombre = asprintf(nombrePlaylist);

    char *values = asprintf(stringify(nombre = "%s"), nombrePlaylist);
    char *condition = asprintf("id = %d", id);
    actualizar_registros("Playlists", values, condition, NULL);
    freem(values);
    freem(condition);

    send_message("Nombre de playlist actualizado.");
    return true;
}

/* ================================================================ */
// BLOQUE: agregar_a_playlist — Agregar una canción a una playlist
/* ================================================================ */

message_handler(agregar_a_playlist)
{
    init_data_json();

    const char *id_playlist = get_string(get_array_idx(data, 0));
    const char *id_cancion = get_string(get_array_idx(data, 1));

    int argc = 2;
    const char *argv[] = {id_playlist, id_cancion};
    char **msg = arg;

    bool success = VibeCast_AgregarAPlaylist(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    end_data_json();
}

static interfaz(AgregarAPlaylist)
{
    int id_playlist = atoi(argv[0]);
    int id_cancion = atoi(argv[1]);

    Playlist *playlist = searchValueInLista(usuario->playlists, &id_playlist, cmpPlaylistConId);
    if (!playlist)
    {
        send_message("Playlist no encontrada.");
        return false;
    }

    Cancion *cancion = searchValueInLista(playlist->canciones, &id_cancion, cmpCancionConId);
    if (cancion)
    {
        send_message("La canción ya está en la playlist.");
        return false;
    }

    cancion = searchValueInLista(canciones, &id_cancion, cmpCancionConId);
    if (!cancion)
    {
        send_message("No se encontró la canción.");
        return false;
    }

    insertValueInLista(playlist->canciones, cancion);

    char *values = asprintf("%d, %d", id_playlist, id_cancion);
    nuevo_registro("Playlist_Canciones", "id_playlist, id_cancion", values, NULL);
    freem(values);

    send_message("Canción agregada a playlist.");
    return true;
}

/* ================================================================ */
// BLOQUE: elminar_de_playlist — Eliminar una canción de una playlist
/* ================================================================ */

message_handler(eliminar_de_playlist)
{
    init_data_json();

    const char *id_playlist = get_string(get_array_idx(data, 0));
    const char *id_cancion = get_string(get_array_idx(data, 1));

    int argc = 2;
    const char *argv[] = {id_playlist, id_cancion};
    char **msg = arg;

    bool success = VibeCast_EliminarDePlaylist(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    end_data_json();
}

static interfaz(EliminarDePlaylist)
{
    int id_playlist = atoi(argv[0]);
    int id_cancion = atoi(argv[1]);

    Playlist *playlist = searchValueInLista(usuario->playlists, &id_playlist, cmpPlaylistConId);
    if (!playlist)
    {
        send_message("Playlist no encontrada.");
        return false;
    }

    Cancion *cancion = deleteValueInLista(playlist->canciones, &id_cancion, cmpCancionConId);
    if (cancion)
    {
        send_message("La canción no está en la playlist.");
        return false;
    }

    char *condition = asprintf("id_playlist = %d AND id_cancion = %d", id_playlist, id_cancion);
    eliminar_registros("Playlist_Canciones", condition, NULL);
    freem(condition);

    send_message("Canción eliminada de playlist.");
    return true;
}
