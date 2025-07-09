#include <ui/interfaces.h>

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

// new_operfn(findPlaylistById)
// {
//     Playlist *p = val;
//     int *targetId = arg;
//     if (p->id == *targetId)
//     {
//         *(Playlist **)arg = p;
//         return FOREACH_BREAK;
//     }

//     return FOREACH_CONTINUE;
// }

// Convierte las canciones de una playlist en JSON
new_operfn(getPlaylistSongsJSON)
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

// Convierte una playlist en JSON
new_operfn(getPlaylistJSON)
{
    Playlist *playlist = val;
    json_object *playlist_json = json_object_new_object();

    json_object_object_add(playlist_json, "id", json_object_new_int(playlist->id));
    json_object_object_add(playlist_json, "name", json_object_new_string(playlist->nombre));
    json_object_object_add(playlist_json, "songCount", json_object_new_int(getListaLength(playlist->canciones)));

    json_object_array_add(arg, playlist_json);

    return FOREACH_CONTINUE;
}

message_handler(get_playlists)
{
    json_object *response = json_object_new_object();
    json_object *playlists_array = json_object_new_array();
    json_object *songs_array = json_object_new_object();

    forEachInLista(usuario->playlists, getPlaylistJSON, playlists_array);
    forEachInLista(usuario->playlists, getPlaylistSongsJSON, songs_array);

    json_object_object_add(response, "playlists", playlists_array);
    json_object_object_add(response, "playlistSongs", songs_array);

    VibeCast_SendJSON(id, HTTP_OK, response, "Playlists cargadas", STATE_SUCCESS);
    json_object_put(response);
}