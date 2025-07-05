#include <ui/interfaces.h>


json_bool *playlist_to_json(Playlist *playlist)
{
    json_object *p = json_object_new_object();

    json_object_object_add(
        p,
        "name",
        json_object_new_string(
            playlist->nombre));
    
    int songCount=getListaLength(playlist->canciones);
    json_object_object_add(
        p,
        "songCount",
        json_object_new_int(songCount)


    );



    return p;
}

new_operfn(playlistsJSON)
{
    json_object_array_add(arg, playlist_to_json(val));
    return FOREACH_CONTINUE;
}

message_handler(get_playlists)
{
    json_object *array = json_object_new_array();
    forEachInLista(usuario->playlists, playlistsJSON, array);
    VibeCast_SendArray(id, HTTP_OK, array, "Playlist cargadas", STATE_SUCCESS);
    
}