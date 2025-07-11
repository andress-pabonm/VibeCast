#include <ui/interfaces.h>

static new_cmpfn(cmpPlaylistConNombre)
{
    const Playlist *a = val_1; // Playlist a comparar
    const char *n = val_2;     // Nombre de la playlist a comparar

    return strcmp(a->nombre, n); // Compara el nombre de la playlist con el nombre dado
}

new_operfn(restar_popularidad)
{
    Cancion *cancion = val;  // Canción actual
    cancion->popularidad--;  // Restamos la popularidad de la canción eliminada
    return FOREACH_CONTINUE; // constante que indica que se sigue iterando con el bucle de reccorer la lista
}

new_operfn(mostrar_canciones)
{
    Cancion *cancion = val; // Canción actual

    printf("[%d]: %s\n", idx, cancion->nombre); // Muestra cancion actual

    return FOREACH_CONTINUE; // Para seguir con el bucle
}

interfaz(crearPlaylist)
{
    const char *nombre = argv[0]; // Nombre de la playlist a crear
    if (!nombre) {
        send_message("Error: Falta nombre de playlist\n");
        return false;
    }

    Playlist *nuevaPlaylist = newPlaylist(); // Creamos una nueva playlist
    nuevaPlaylist->nombre = asprintf(nombre);
    insertValueInLista(usuario->playlists, nuevaPlaylist); // Insertamos la nueva playlist en la lista de playlists del usuario

    return true;
}

message_handler(crearPlaylist)
{
    init_data_json();
    const char *nombre = get_string(get_array_idx(data, 0));

    const char *argv[] = {nombre};

    char **msg = arg;
    bool success = VibeCast_crearPlaylist(usuario, 1, argv, msg);

    VibeCast_SendText(id, HTTP_OK, *msg,
                      success ? "Playlist creada" : "Error al crear playlist",
                      STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

//-------------------------------------------------------------

interfaz(agregarCancionPlaylist)
{
    const char *nombrePlaylist = argv[0]; // Nombre de la playlist a la que se agregará la canción
    int IdCancion = atoi(argv[1]); // ID de la canción a agregar

    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre);

    if (!playlist)
    {
        send_message("Error: La playlist '%s' no existe\n", nombrePlaylist);

        return false;
    }

    Cancion *cancion = searchValueInLista(canciones, &IdCancion, cmpCancionConId);
    // verificamos si la cancion existe
    if (!cancion)
    {
        send_message("Error: La canción con ID %d no existe\n", IdCancion);
        return false;
    }

    // Agrega la canción a la lista de canciones de la playlist
    insertValueInLista(playlist->canciones, cancion);
    return true;
}

message_handler(agregarCancionPlaylist)
{
    init_data_json();
    const char *nombrePlaylist = get_string(get_array_idx(data, 0));
    int idCancion = get_int(get_array_idx(data, 1));

    char idBuffer[12];
    snprintf(idBuffer, sizeof(idBuffer), "%d", idCancion);
    const char *argv[] = {nombrePlaylist, idBuffer};

    char **msg = arg;
    bool success = VibeCast_agregarCancionPlaylist(usuario, 2, argv, msg);

    VibeCast_SendText(id, HTTP_OK, *msg,
                      success ? "Canción agregada" : "Error al agregar canción",
                      STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

// ---------------------------------------------------------------------------------------------

interfaz( eliminarCancionPlaylist)
{   
    const char *nombrePlaylist = argv[0];  // Nombre de la playlist
    int Idcancion = atoi(argv[1]); // ID de la canción a eliminar

    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre);

    if (!playlist)
    {
        send_message("Error: La playlist '%s' no existe\n", nombrePlaylist);
        return false;
    }
    
    Cancion *cancionEliminada = deleteValueInLista(playlist->canciones, &Idcancion, cmpCancionConId);

    if (cancionEliminada == NULL)
    {
        send_message("Error: La canción con ID %d no se encuentra en la playlist\n", Idcancion);
        return false;
    }

    send_message("Canción %s eliminada de la playlist '%s'\n", cancionEliminada->nombre, playlist->nombre);
    // Restamos la popularidad de la canción eliminada
    cancionEliminada->popularidad--;

    return true;
}

message_handler(eliminarCancionPlaylist)
{
    init_data_json();
    const char *nombrePlaylist = get_string(get_array_idx(data, 0));
    int idCancion = get_int(get_array_idx(data, 1));

    char idBuffer[12];
    snprintf(idBuffer, sizeof(idBuffer), "%d", idCancion);
    const char *argv[] = {nombrePlaylist, idBuffer};

    char **msg = arg;
    bool success = VibeCast_eliminarCancionPlaylist(usuario, 2, argv, msg);

    VibeCast_SendText(id, HTTP_OK, *msg,
                      success ? "Canción eliminada" : "Error al eliminar canción",
                      STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

// ---------------------------------------------------------------------------------------------

interfaz(eliminarPlaylist)
{
    const char *nombrePlaylist = argv[0]; // Nombre de la playlist a eliminar

    // Buscamos la playlist por nombre
    Playlist *playlisteliminada = deleteValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre); // Eliminamos la playlist de lalista de playlists
    if (playlisteliminada == NULL)
    {
        send_message("Error: No se pudo eliminar la playlist '%s'\n", nombrePlaylist);
        return false;
    }

    send_message("Playlist '%s' eliminada correctamente\n", nombrePlaylist);
    // Liberamos la memoria asociada a la playlist
    freem(playlisteliminada->nombre); // Liberamos el nombre de la playlist

    destroyLista(playlisteliminada->canciones, restar_popularidad, NULL); // Eliminamos la playlist y todas las canciones eliminamos una popularidad

    freem(playlisteliminada);

    return true;
}

message_handler(eliminarPlaylist)
{
    init_data_json();
    const char *nombre = get_string(get_array_idx(data, 0));
    const char *argv[] = {nombre};

    char **msg = arg;
    bool success = VibeCast_eliminarPlaylist(usuario, 1, argv, msg);

    VibeCast_SendText(id, HTTP_OK, *msg,
                      success ? "Playlist eliminada" : "Error al eliminar playlist",
                      STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

// ---------------------------------------------------------------------------------------------


interfaz(mostrarCancionesPlaylist)
{
    const char *nombrePlaylist = argv[0]; // Nombre de la playlist a mostrar

    Playlist *playlist = searchValueInLista(usuario->playlists, nombrePlaylist, cmpPlaylistConNombre); // Buscamos la playlist por nombre

    if (!playlist)
    {
       send_message("Error: La playlist '%s' no existe\n", nombrePlaylist);

        return false; // Retornar para evitar errores con las siguientes instrucciones
    }

    send_message("Playlist: %s\n", playlist->nombre);

    forEachInLista(playlist->canciones, mostrar_canciones, NULL);
    return true;
}

message_handler(mostrarCancionesPlaylist)
{
    init_data_json();
    const char *nombre = get_string(get_array_idx(data, 0));
    const char *argv[] = {nombre};

    char **msg = arg;
    bool success = VibeCast_mostrarCancionesPlaylist(usuario, 1, argv, msg);

    VibeCast_SendText(id, HTTP_OK, *msg,
                      success ? "Mostrando canciones" : "Error al mostrar canciones",
                      STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}
