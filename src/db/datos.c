#include <db/datos.h>
#include <db/dbmgr.h>
#include <utils/utils.h>
#include <string.h>
#include <stdio.h>

static ABB usuarios = {NULL};
static ABB artistas = {NULL};
static Lista canciones = {NULL};
static Cola anuncios = {NULL, NULL};

#define new_cmp(name) int name(void *value_ptr_1, void *value_ptr_2)
#define new_free(name) bool name(void *value_ptr)
#define new_op(name) bool name(int index, void *value_ptr)

new_cmp(cmpUsuarios)
{
    Usuario *usuario_1 = value_ptr_1;
    Usuario *usuario_2 = value_ptr_2;

    return strcmp(usuario_1->username, usuario_2->username);
}

new_cmp(cmpUsuarioConUsername)
{
    Usuario *usuario = value_ptr_1;
    char *username = value_ptr_2;

    return strcmp(usuario->username, username);
}

new_cmp(cmpArtistas)
{
    Artista *artista_1 = value_ptr_1;
    Artista *artista_2 = value_ptr_2;

    return strcmp(artista_1->nombreArtista, artista_2->nombreArtista);
}

new_cmp(cmpCancionConId)
{
    Cancion *cancion = value_ptr_1;
    int *id = value_ptr_2;

    return cancion->id - *id;
}

// Para rellenar el árbol de usuarios
new_select_handler(cargarUsuarios);

// Para rellenar la lista de álbumes por artista
new_select_handler(cargarAlbumesPorArtista);

// Para rellenar la lista de canciones por artista. Además, se rellena la lista de canciones general.
new_select_handler(cargarCancionesPorAlbum);

// Para cargar datos por usuario (amigos, playlists e historial)
new_select_handler(cargarDatosPorUsuario);

// Para cargar los amigos
new_select_handler(cargarAmigosPorUsuario);

// Para cargar las playlists
new_select_handler(cargarPlaylistsPorUsuario);
new_select_handler(cargarCancionesPorPlaylist);

// Para cargar los historiales
new_select_handler(cargarHistorialPorUsuario);
new_select_handler(cargarReproduccionesPorHistorial);

// Para cargar los anuncios
new_select_handler(cargarAnuncios);

bool func(LoadData)
{
    char *condition = NULL;

    // TODO 1: Cargar usuarios, artistas, álbumes y canciones
    obtener_registros("Usuarios", "*", NULL, cargarUsuarios, NULL, NULL);

    // TODO 2: Cargar amigos, playlists e historiales de reproducción
    obtener_registros("Usuarios", "username", NULL, cargarDatosPorUsuario, NULL, NULL);

    // TODO 3: Cargar anuncios
    obtener_registros("Anuncios", "*", NULL, cargarAnuncios, NULL, NULL);

    return true; // Datos cargados correctamente
}

new_select_handler(cargarUsuarios)
{
    Usuario usuario;

    // TODO 1: Copiar datos a la variable (usuario)
    usuario.email = malloc_cpy(strlen(argv[0]) + 1, argv[0]);
    usuario.password = malloc_cpy(strlen(argv[0]) + 1, argv[1]);
    usuario.username = malloc_cpy(strlen(argv[0]) + 1, argv[2]);
    usuario.nickname = malloc_cpy(strlen(argv[0]) + 1, argv[3]);
    usuario.pais = malloc_cpy(strlen(argv[0]) + 1, argv[4]);
    sscanf(argv[5], "%d", &usuario.plan);

    usuario.amigos.head = NULL;
    usuario.playlists.head = NULL;

    usuario.historial.reproducciones.top = NULL;
    usuario.historial.cantidadAnuncios = 0;
    usuario.historial.tiempoEscuchado = 0;

    usuario.artista = NULL;

    Usuario *ptr_usuario = alloc(Usuario, &usuario);

    insertarNodo(ABB, &usuarios, ptr_usuario, cmpUsuarios);

    if (strlen(argv[6]))
    {
        // Cargar artista
        Artista artista;
        artista.nombreArtista = malloc_cpy(strlen(argv[6] + 1), argv[6]);
        artista.albumes.head = NULL;

        Artista *ptr_artista = alloc(Artista, &artista);
        insertarNodo(ABB, &artistas, ptr_artista, cmpArtistas);

        ptr_usuario->artista = ptr_artista;

        // Cargar álbumes
        char *condition = mprintf(stringify(artista = "%s"), usuario.username);
        obtener_registros("Albumes", "id, nombre_album, fecha_creacion", condition, cargarAlbumesPorArtista, ptr_artista, NULL);
        sqlite3_free(condition);
    }

    return 0;
}

// Para rellenar la lista de álbumes por artista
new_select_handler(cargarAlbumesPorArtista)
{
    Album album;

    album.artista = arg;
    album.nombreAlbum = malloc_cpy(strlen(argv[1]) + 1, argv[2]);
    strcpy(album.fechaCreacion, argv[2]);
    album.canciones.head = NULL;

    Album *ptr_album = alloc(Album, &album);
    insertarNodo(Lista, &album.artista->albumes, ptr_album, NULL);

    char *condition = mprintf("id_album = %s", argv[0]);
    obtener_registros("Canciones", "*", condition, cargarCancionesPorAlbum, ptr_album, NULL);
    sqlite3_free(condition);

    return 0;
}

// Para rellenar la lista de canciones por artista. Además, se rellena la lista de canciones general.
new_select_handler(cargarCancionesPorAlbum)
{
    Cancion cancion;

    sscanf(argv[0], "%d", &cancion.id);
    cancion.album = arg;
    cancion.nombreCancion = malloc_cpy(strlen(argv[2]) + 1, argv[2]);
    cancion.genero = malloc_cpy(strlen(argv[3]) + 1, argv[3]);
    strcpy(cancion.fechaLanzamiento, argv[4]);
    sscanf(argv[5], "%d", &cancion.popularidad);
    sscanf(argv[6], "%d", &cancion.reproducciones);
    sscanf(argv[7], "%f", &cancion.duracion);
    cancion.url = malloc_cpy(strlen(argv[8]) + 1, argv[8]);

    Cancion *ptr_cancion = alloc(Cancion, &cancion);
    insertarNodo(Lista, &cancion.album->canciones, ptr_cancion, NULL);
    insertarNodo(Lista, &canciones, ptr_cancion, NULL);

    return 0;
}

// Para cargar datos por usuario (amigos, playlists e historial)
new_select_handler(cargarDatosPorUsuario)
{
    Usuario *usuario = (*buscarNodo(ABB, &usuarios, argv[0], cmpUsuarioConUsername))->value_ptr;

    char *condition = mprintf(stringify(usuario1 = "%s"), argv[0]);
    obtener_registros("Amigos", "usuario2", condition, cargarAmigosPorUsuario, &usuario->amigos, NULL);
    sqlite3_free(condition);

    condition = mprintf(stringify(propietario = "%s"), argv[0]);
    obtener_registros("Playlists", "*", condition, cargarPlaylistsPorUsuario, &usuario->playlists, NULL);
    sqlite3_free(condition);

    condition = mprintf(stringify(username = "%s"), argv[0]);
    obtener_registros("Historiales", "*", condition, cargarHistorialPorUsuario, &usuario->historial, NULL);
    sqlite3_free(condition);

    return 0;
}

// Para cargar los amigos
new_select_handler(cargarAmigosPorUsuario)
{
    Usuario *amigo = (*buscarNodo(ABB, &usuarios, argv[0], cmpUsuarioConUsername))->value_ptr;
    insertarNodo(Lista, arg, amigo, NULL);
    return 0;
}

// Para cargar las playlists
new_select_handler(cargarPlaylistsPorUsuario)
{
    Playlist playlist;

    playlist.nombrePlaylist = malloc_cpy(strlen(argv[1]) + 1, argv[1]);
    playlist.canciones.head = NULL;

    Playlist *ptr_playlist = alloc(Playlist, &playlist);
    insertarNodo(Lista, arg, ptr_playlist, NULL);

    char *condition = mprintf("id_playlist = %s", argv[0]);
    obtener_registros("Playlist_Canciones", "id_cancion", condition, cargarCancionesPorPlaylist, &ptr_playlist->canciones, NULL);
    sqlite3_free(condition);

    return 0;
}

new_select_handler(cargarCancionesPorPlaylist)
{
    int id_cancion;
    sscanf(argv[0], "%d", &id_cancion);
    Cancion *cancion = (*buscarNodo(Lista, &canciones, &id_cancion, cmpCancionConId))->value_ptr;
    insertarNodo(Lista, arg, cancion, NULL);
    return 0;
}

// Para cargar los historiales
new_select_handler(cargarHistorialPorUsuario)
{
    Historial *historial = arg;

    sscanf(argv[1], "", &historial->tiempoEscuchado);
    sscanf(argv[2], "", &historial->cantidadAnuncios);

    char *condition = mprintf(stringify(username = "%s"), argv[0]);
    obtener_registros("Reproducciones", "id_cancion, fecha_escuchado", condition, cargarReproduccionesPorHistorial, &historial->reproducciones, NULL);
    sqlite3_free(condition);

    return 0;
}

new_select_handler(cargarReproduccionesPorHistorial)
{
    Reproduccion reproduccion;

    int id_cancion;
    sscanf(argv[0], "%d", &id_cancion);
    reproduccion.cancion = (*buscarNodo(Lista, &canciones, &id_cancion, cmpCancionConId))->value_ptr;

    strcpy(reproduccion.fechaEscuchado, argv[1]);

    Reproduccion *ptr_reproduccion = alloc(Reproduccion, &reproduccion);
    insertarNodo(Pila, arg, ptr_reproduccion);

    return 0;
}

new_select_handler(cargarAnuncios)
{
    Anuncio anuncio;

    anuncio.anunciante = (*buscarNodo(ABB, &usuarios, argv[0], cmpUsuarioConUsername))->value_ptr;
    anuncio.url = malloc_cpy(strlen(argv[1]) + 1, argv[1]);

    Anuncio *ptr_anuncio = alloc(Anuncio, &anuncio);
    insertarNodo(Cola, &anuncios, ptr_anuncio);

    return 0;
}
