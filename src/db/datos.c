#include <db/datos.h>
#include <db/dbmgr.h>
#include <utils/utils.h>

/* ==== Estructuras de datos ==== */

// ABB para usuarios (ordenado por Usuario.username)
ABB usuarios = {NULL};

// ABB para artistas (ordenado por Artista.nombre)
ABB artistas = {NULL};

// Lista global de canciones (ordenado por Cancion.album)
Lista canciones = {NULL};

// Cola para los anuncios (ordenado por tiempo de creacion)
Cola anuncios = {NULL, NULL};

/* ==== Funciones de ayuda para cargar los datos ==== */

// ==== Declaraciones ====

static select_handler(cargarUsuarios);

static select_handler(cargarArtistaPorUsuario);
static select_handler(cargarAlbumesPorArtista);
static select_handler(cargarCancionesPorAlbum);

static select_handler(cargarAmigosPorUsuario);

static select_handler(cargarPlaylistsPorUsuario);
static select_handler(cargarCancionesPorPlaylist);

static select_handler(cargarHistorialPorUsuario);
static select_handler(cargarReproduccionesPorHistorial);

static select_handler(cargarAnuncios);

static select_handler(cargarOtrosDatosPorUsuario);

// ==== Definiciones ====

#define USUARIO_ID 0
#define USUARIO_USERNAME 1
#define USUARIO_EMAIL 2
#define USUARIO_PASSWORD 3
#define USUARIO_NICKNAME 4
#define USUARIO_PAIS 5
#define USUARIO_PLAN 6

static select_handler(cargarUsuarios)
{
    // Cargar la información del usuario

    Usuario *usuario = alloc(Usuario, NULL);

    usuario->username = mprintf(argv[USUARIO_USERNAME]);
    usuario->email = mprintf(argv[USUARIO_EMAIL]);
    usuario->password = mprintf(argv[USUARIO_PASSWORD]);
    usuario->nickname = mprintf(argv[USUARIO_NICKNAME]);
    usuario->pais = mprintf(argv[USUARIO_PAIS]);
    sscanf(argv[USUARIO_PLAN], "%d", &usuario->plan);

    usuario->artista = NULL;

    usuario->amigos.head = NULL;
    usuario->playlists.head = NULL;

    usuario->historial.reproducciones.top = NULL;
    usuario->historial.tiempoEscuchado = 0;
    usuario->historial.cantidadAnuncios = 0;

    // Insertarlo en el árbol
    insertarNodo(ABB, &usuarios, usuario, cmpUsuarios);

    // Cargar su perfil de artista si tiene

    char *condition =
        mprintf("id_usuario = %s", argv[USUARIO_ID]);

    obtener_registros(
        "Artistas", "*", condition,
        cargarArtistaPorUsuario,
        usuario, NULL);

    free(condition);

    return 0;
}

#define ARTISTA_ID 0
#define ARTISTA_NOMBRE 1

static select_handler(cargarArtistaPorUsuario)
{
    // Cargar información del artista

    Artista *artista = alloc(Artista, NULL);

    artista->usuario = arg; // Enlazar a su usuario
    artista->nombre = mprintf(argv[ARTISTA_NOMBRE]);
    artista->albumes.head = NULL;

    // Insertarlo en el árbol
    insertarNodo(ABB, &artistas, artista, cmpArtistas);

    // Enlazar sus álbumes

    char *condition =
        mprintf("id_artista = %s", argv[ARTISTA_ID]);

    obtener_registros(
        "Albumes", "*", condition,
        cargarAlbumesPorArtista,
        artista, NULL);

    free(condition);

    // Enlazar a su perfil de usuario

    artista->usuario->artista = artista;

    return 0;
}

#define ALBUM_ID 0
#define ALBUM_NOMBRE 1
#define ALBUM_FECHA_CREACION 2

static select_handler(cargarAlbumesPorArtista)
{
    // Cargar información del álbum

    Album *album = alloc(Album, NULL);

    sscanf(argv[ALBUM_ID], "%d", &album->id);
    album->artista = arg; // Enlazar al artista
    album->nombre = mprintf(argv[ALBUM_NOMBRE]);
    album->fechaCreacion = mprintf(argv[ALBUM_FECHA_CREACION]);
    album->canciones.head = NULL;

    // Insertarlo en la lista
    insertarNodo(
        Lista, &album->artista->albumes,
        album, NULL);

    // Enlazar sus canciones

    char *condition =
        mprintf("id_album = %d", album->id);

    obtener_registros(
        "Canciones", "*",
        condition, cargarCancionesPorAlbum,
        album, NULL);

    free(condition);

    return 0;
}

#define CANCION_ID 0
#define CANCION_NOMBRE 1
#define CANCION_GENERO 2
#define CANCION_FECHA_PUBLICACION 3
#define CANCION_DURACION 4
#define CANCION_URL 5
#define CANCION_POPULARIDAD 6
#define CANCION_REPRODUCCIONES 7

static select_handler(cargarCancionesPorAlbum)
{
    // Cargar la información de la canción

    Cancion *cancion = alloc(Cancion, NULL);

    cancion->album = arg; // Enlazar a su álbum
    sscanf(argv[CANCION_ID], "%d", &cancion->id);
    cancion->nombre = mprintf(argv[CANCION_NOMBRE]);
    cancion->genero = mprintf(argv[CANCION_GENERO]);
    cancion->fechaPublicacion = mprintf(argv[CANCION_FECHA_PUBLICACION]);

    sscanf(argv[CANCION_DURACION], "%d", &cancion->duracion);
    cancion->url = mprintf(argv[CANCION_URL]);

    sscanf(argv[CANCION_POPULARIDAD], "%d", &cancion->popularidad);
    sscanf(argv[CANCION_REPRODUCCIONES], "%d", &cancion->reproducciones);

    // Insertar en la lista del álbum
    insertarNodo(
        Lista, &cancion->album->canciones,
        cancion, NULL);
    // Insertar en la lista global
    insertarNodo(
        Lista, &canciones,
        cancion, NULL);

    return 0;
}

#define AMIGO_USERNAME 0

static select_handler(cargarAmigosPorUsuario)
{
    // Buscar el amigo

    Usuario *amigo =
        (*buscarNodo(
             ABB, &usuarios,
             argv[AMIGO_USERNAME],
             cmpUsuarioConUsername))
            ->value_ptr;

    // Insertarlo en la lista de amigos
    insertarNodo(Lista, arg, amigo, NULL);

    return 0;
}

#define PLAYLIST_ID 0
#define PLAYLIST_NOMBRE 1

static select_handler(cargarPlaylistsPorUsuario)
{
    // Cargar la información de la playlist

    Playlist *playlist = alloc(Playlist, NULL);

    sscanf(argv[PLAYLIST_ID], "%d", &playlist->id);
    playlist->nombre = mprintf(argv[PLAYLIST_NOMBRE]);
    playlist->canciones.head = NULL;

    // Insertarlo en la lista correspondiente
    insertarNodo(Lista, arg, playlist, NULL);

    // Enlazar las canciones

    char *condition =
        mprintf("id_playlist = %d", playlist->id);

    obtener_registros(
        "Playlist_Canciones", "id_cancion",
        condition, cargarCancionesPorPlaylist,
        &playlist->canciones, NULL);

    free(condition);

    return 0;
}
static select_handler(cargarCancionesPorPlaylist)
{
    int id_cancion;
    sscanf(argv[CANCION_ID], "%d", &id_cancion);

    // Buscar canción
    Cancion *cancion =
        (*buscarNodo(
             Lista, &canciones,
             &id_cancion, cmpCancionConId))
            ->value_ptr;

    // Insertarla en la playlist
    insertarNodo(Lista, arg, cancion, NULL);

    return 0;
}

#define HISTORIAL_TIEMPO_ESCUCHADO 1
#define HISTORIAL_CANTIDAD_ANUNCIOS 2

static select_handler(cargarHistorialPorUsuario)
{
    // Cargar los datos del historial

    Historial *historial = arg;

    sscanf(argv[HISTORIAL_TIEMPO_ESCUCHADO], "%d", &historial->tiempoEscuchado);
    sscanf(argv[HISTORIAL_CANTIDAD_ANUNCIOS], "%d", &historial->cantidadAnuncios);

    // Enlazar reproducciones

    char *condition =
        mprintf("usuario_id = %s", argv[USUARIO_ID]);

    obtener_registros(
        "Reproducciones", "id_cancion, fecha_escuchado", condition,
        cargarReproduccionesPorHistorial,
        &historial->reproducciones, NULL);

    free(condition);

    return 0;
}

#define REPRODUCCION_FECHA_ESCUCHADO 1

static select_handler(cargarReproduccionesPorHistorial)
{
    // Cargar la información de las reproducciones

    int id_cancion;
    sscanf(argv[CANCION_ID], "%d", &id_cancion);

    Reproduccion *reproduccion = alloc(Reproduccion, NULL);

    reproduccion->fechaEscuchado =
        mprintf(argv[REPRODUCCION_FECHA_ESCUCHADO]);

    reproduccion->cancion =
        (*buscarNodo(
             Lista, &canciones,
             &id_cancion, cmpCancionConId))
            ->value_ptr;

    // Insertarlo en el historial
    insertarNodo(Pila, arg, reproduccion);

    return 0;
}

#define ANUNCIO_ANUNCIANTE 0
#define ANUNCIO_URL 1

static select_handler(cargarAnuncios)
{
    // Cargar la información de los anuncios

    Anuncio *anuncio = alloc(Anuncio, NULL);

    anuncio->url = mprintf(argv[ANUNCIO_URL]);
    anuncio->anunciante =
        (*buscarNodo(
             ABB, &usuarios,
             argv[ANUNCIO_ANUNCIANTE],
             cmpUsuarioConUsername))
            ->value_ptr;

    // Insertar en la cola de anuncios
    insertarNodo(Cola, &anuncios, anuncio);

    return 0;
}

static select_handler(cargarOtrosDatosPorUsuario)
{
    // Buscar usuario

    int id_usuario;
    sscanf(argv[USUARIO_ID], "%d", &id_usuario);

    Usuario *usuario =
        (*buscarNodo(
             ABB, &usuarios,
             argv[USUARIO_USERNAME],
             cmpUsuarioConUsername))
            ->value_ptr;

    // Cargar amigos

    char *condition = mprintf("Amigos.id_usuario_1 = %d", id_usuario);

    obtener_registros(
        "Amigos JOIN Usuarios ON Usuarios.id = Amigos.id_usuario_2",
        "Usuarios.username",
        condition,
        cargarAmigosPorUsuario,
        &usuario->amigos, NULL);

    free(condition);

    // Cargar playlists

    condition = mprintf("id_usuario = %d", id_usuario);

    obtener_registros(
        "Playlist", "id, nombre", condition,
        cargarPlaylistsPorUsuario,
        &usuario->playlists, NULL);

    // Cargar historial

    obtener_registros(
        "Historiales", "*", condition,
        cargarHistorialPorUsuario,
        &usuario->historial, NULL);

    free(condition);

    return 0;
}

/* ======== Funciones visibles en otros archivos ======== */

new_op(mostrarUsuario)
{
    Usuario *usuario = value_ptr;

    printf("[%d]: %s\n", index, usuario->username);

    return true;
}

bool func(LoadData)
{
    char *errmsg = NULL;

    // Iniciar la base de datos
    if (!func(InitDB, "data.db", "db_setup.sql", &errmsg))
    {
        printf("Error al inicializar la base de datos: %s\n", errmsg);
        free_errmsg(errmsg);
        return false;
    }

    // Cargar usuarios, artistas, álbumes y canciones
    obtener_registros(
        "Usuarios", "*", NULL,
        cargarUsuarios, NULL, NULL);

    // Cargar amigos, playlists, historiales
    obtener_registros(
        "Usuarios", "id, username", NULL,
        cargarOtrosDatosPorUsuario, NULL, NULL);

    // Cargar anuncios
    obtener_registros(
        "Anuncios JOIN Usuarios ON Usuarios.id = Anuncios.id_usuario",
        "Usuarios.username, Anuncios.url", NULL, cargarAnuncios, NULL, NULL);

    return true; // Datos cargados correctamente
}

bool func(FreeData)
{
    /* Aquí se libera la memoria de las estructuras de datos */

    return true;
}

/* ======== Comparadores ======== */

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

    return strcmp(artista_1->nombre, artista_2->nombre);
}

new_cmp(cmpCancionConId)
{
    Cancion *cancion = value_ptr_1;
    int *id = value_ptr_2;

    return cancion->id - *id;
}
