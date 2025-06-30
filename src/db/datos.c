#include <db/datos.h>
#include <db/dbmgr.h>
#include <utils/utils.h>

/* ==== Estructuras de datos ==== */

// ABB para usuarios (ordenado por Usuario.username)
ABB usuarios = NULL;

// ABB para artistas (ordenado por Artista.nombre)
ABB artistas = NULL;

// Lista global de canciones (ordenado por Cancion.album)
Lista canciones = NULL;

// Cola para los anuncios (ordenado por tiempo de creacion)
Cola anuncios = NULL;

/* ==== Funciones para ordenar las colecciones ==== */

static new_cmpfn(cmpUsuarios)
{
    const Usuario *usuario_1 = value_ptr_1;
    const Usuario *usuario_2 = value_ptr_2;

    return strcmp(usuario_1->username, usuario_2->username);
}

static new_cmpfn(cmpArtistas)
{
    const Artista *artista_1 = value_ptr_1;
    const Artista *artista_2 = value_ptr_2;

    return strcmp(artista_1->nombre, artista_2->nombre);
}

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
    Usuario *usuario = newUsuario();
    if (!usuario)
        return 1;

    usuario->username = asprintf(argv[USUARIO_USERNAME]);
    usuario->email = asprintf(argv[USUARIO_EMAIL]);
    usuario->password = asprintf(argv[USUARIO_PASSWORD]);
    usuario->nickname = asprintf(argv[USUARIO_NICKNAME]);
    usuario->pais = asprintf(argv[USUARIO_PAIS]);
    sscanf(argv[USUARIO_PLAN], "%d", &usuario->plan);

    insertValueInABB(usuarios, usuario);

    char *condition = asprintf("id_usuario = %s", argv[USUARIO_ID]);
    obtener_registros("Artistas", "*", condition, cargarArtistaPorUsuario, usuario, NULL);
    freem(condition);

    return 0;
}

#define ARTISTA_ID 0
#define ARTISTA_NOMBRE 1

static select_handler(cargarArtistaPorUsuario)
{
    Artista *artista = newArtista();
    if (!artista)
        return 1;

    cast(Usuario *, arg)->artista = artista;

    artista->usuario = arg;
    artista->nombre = asprintf(argv[ARTISTA_NOMBRE]);

    insertValueInABB(artistas, artista);

    char *condition = asprintf("id_artista = %s", argv[ARTISTA_ID]);
    obtener_registros("Albumes", "*", condition, cargarAlbumesPorArtista, artista, NULL);
    freem(condition);

    return 0;
}

#define ALBUM_ID 0
#define ALBUM_NOMBRE 1
#define ALBUM_FECHA_CREACION 2

static select_handler(cargarAlbumesPorArtista)
{
    Album *album = newAlbum();
    if (!album)
        return 1;

    insertValueInLista(cast(Artista *, arg)->albumes, album);

    album->artista = arg;
    sscanf(argv[ALBUM_ID], "%d", &album->id);
    album->nombre = asprintf(argv[ALBUM_NOMBRE]);
    album->fechaCreacion = asprintf(argv[ALBUM_FECHA_CREACION]);

    char *condition = asprintf("id_album = %d", album->id);
    obtener_registros("Canciones", "*", condition, cargarCancionesPorAlbum, album, NULL);
    freem(condition);

    return 0;
}

#define CANCION_ID 0
#define CANCION_NOMBRE 2
#define CANCION_GENERO 3
#define CANCION_FECHA_PUBLICACION 4
#define CANCION_DURACION 5
#define CANCION_URL 6
#define CANCION_POPULARIDAD 7
#define CANCION_REPRODUCCIONES 8

static select_handler(cargarCancionesPorAlbum)
{
    Cancion *cancion = newCancion();
    if (!cancion)
        return 1;

    insertValueInLista(cast(Album *, arg)->canciones, cancion);
    insertValueInLista(canciones, cancion);

    cancion->album = arg;
    sscanf(argv[CANCION_ID], "%d", &cancion->id);
    cancion->nombre = asprintf(argv[CANCION_NOMBRE]);
    cancion->genero = asprintf(argv[CANCION_GENERO]);
    cancion->fechaPublicacion = asprintf(argv[CANCION_FECHA_PUBLICACION]);
    sscanf(argv[CANCION_DURACION], "%d", &cancion->duracion);
    cancion->url = asprintf(argv[CANCION_URL]);
    sscanf(argv[CANCION_POPULARIDAD], "%d", &cancion->popularidad);
    sscanf(argv[CANCION_REPRODUCCIONES], "%d", &cancion->reproducciones);

    return 0;
}

#define AMIGO_USERNAME 0

static select_handler(cargarAmigosPorUsuario)
{
    // Buscar el amigo

    Usuario *amigo =
        searchValueInABB(
            usuarios,
            argv[AMIGO_USERNAME],
            cmpUsuarioConUsername);

    // Insertarlo en la lista de amigos
    insertValueInLista(arg, amigo);

    return 0;
}

#define PLAYLIST_ID 0
#define PLAYLIST_NOMBRE 1

static select_handler(cargarPlaylistsPorUsuario)
{
    Playlist *playlist = newPlaylist();
    if (!playlist)
        return 1;

    sscanf(argv[PLAYLIST_ID], "%d", &playlist->id);
    playlist->nombre = asprintf(argv[PLAYLIST_NOMBRE]);

    insertValueInLista(arg, playlist);

    char *condition = asprintf("id_playlist = %d", playlist->id);
    obtener_registros("Playlist_Canciones", "id_cancion", condition, cargarCancionesPorPlaylist, &playlist->canciones, NULL);
    free(condition);

    return 0;
}

static select_handler(cargarCancionesPorPlaylist)
{
    int id_cancion;
    sscanf(argv[CANCION_ID], "%d", &id_cancion);

    // Buscar canción
    Cancion *cancion =
        searchValueInLista(
            canciones,
            &id_cancion,
            cmpCancionConId);

    // Insertarla en la playlist
    insertValueInLista(arg, cancion);

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
        asprintf("usuario_id = %s", argv[USUARIO_ID]);

    obtener_registros(
        "Reproducciones", "id_cancion, fecha_escuchado", condition,
        cargarReproduccionesPorHistorial,
        &historial->reproducciones, NULL);

    freem(condition);

    return 0;
}

#define REPRODUCCION_FECHA_ESCUCHADO 1

static select_handler(cargarReproduccionesPorHistorial)
{
    int id_cancion;
    sscanf(argv[CANCION_ID], "%d", &id_cancion);

    Reproduccion *reproduccion = newReproduccion();
    if (!reproduccion)
        return 1;

    reproduccion->fechaEscuchado = asprintf(argv[REPRODUCCION_FECHA_ESCUCHADO]);
    reproduccion->cancion =
        searchValueInLista(canciones,
                           &id_cancion,
                           cmpCancionConId);

    insertValueInPila(arg, reproduccion);
    return 0;
}

#define ANUNCIO_ANUNCIANTE 0
#define ANUNCIO_URL 1

static select_handler(cargarAnuncios)
{
    Anuncio *anuncio = newAnuncio();
    if (!anuncio)
        return 1;

    anuncio->url = asprintf(argv[ANUNCIO_URL]);
    anuncio->anunciante =
        searchValueInABB(usuarios,
                         argv[ANUNCIO_ANUNCIANTE],
                         cmpUsuarioConUsername);

    insertValueInCola(anuncios, anuncio);
    return 0;
}

static select_handler(cargarOtrosDatosPorUsuario)
{
    // Buscar usuario

    int id_usuario;
    sscanf(argv[USUARIO_ID], "%d", &id_usuario);

    Usuario *usuario =
        searchValueInABB(
            usuarios,
            argv[USUARIO_USERNAME],
            cmpUsuarioConUsername);

    // Cargar amigos

    char *condition = asprintf("Amigos.id_usuario_1 = %d", id_usuario);

    obtener_registros(
        "Amigos JOIN Usuarios ON Usuarios.id = Amigos.id_usuario_2",
        "Usuarios.username",
        condition,
        cargarAmigosPorUsuario,
        usuario->amigos, NULL);

    freem(condition);

    puts("Amigos enlazados");

    // Cargar playlists

    condition = asprintf("id_usuario = %d", id_usuario);

    obtener_registros(
        "Playlist", "id, nombre", condition,
        cargarPlaylistsPorUsuario,
        usuario->playlists, NULL);

    puts("Playlists cargadas");

    // Cargar historial

    obtener_registros(
        "Historiales", "*", condition,
        cargarHistorialPorUsuario,
        &usuario->historial, NULL);

    freem(condition);

    puts("Historial cargado");

    return 0;
}

/* ======== Funciones visibles en otros archivos ======== */

static select_handler(checkData)
{
    puts("\t\tRegistro:");
    for (int i = 0; i < argc; i++)
        printf("\t\t\t[%s]: %s\n", fields[i], argv[i]);
    return 0;
}

static void showData()
{
    puts("Datos cargaods:");

    puts("\tU<suarios:");
    obtener_registros("Usuarios", "*", NULL, checkData, NULL, NULL);

    puts("\tAmigos:");
    obtener_registros("Amigos", "*", NULL, checkData, NULL, NULL);

    puts("\tArtistas:");
    obtener_registros("Artistas", "*", NULL, checkData, NULL, NULL);

    puts("\tAlbumes:");
    obtener_registros("Albumes", "*", NULL, checkData, NULL, NULL);

    puts("\tCanciones:");
    obtener_registros("Canciones", "*", NULL, checkData, NULL, NULL);

    puts("\tPlaylists:");
    obtener_registros("Playlists", "*", NULL, checkData, NULL, NULL);

    puts("\tPlaylist_Canciones:");
    obtener_registros("Playlist_Canciones", "*", NULL, checkData, NULL, NULL);

    puts("\tHistoriales:");
    obtener_registros("Historiales", "*", NULL, checkData, NULL, NULL);

    puts("\tReproducciones:");
    obtener_registros("Reproducciones", "*", NULL, checkData, NULL, NULL);

    puts("\tAnuncios:");
    obtener_registros("Anuncios", "*", NULL, checkData, NULL, NULL);
}

bool func(LoadData)
{
    usuarios = newABB(cmpUsuarios);
    artistas = newABB(cmpArtistas);
    canciones = newLista(NULL);
    anuncios = newCola();

    if (!usuarios || !artistas || !canciones || !anuncios)
        return false;

    puts("Variables globales inicializadas");

    char *errmsg = NULL;

    // Iniciar la base de datos
    if (!func(InitDB, "data.db", "db_setup.sql", &errmsg))
    {
        printf("Error al inicializar la base de datos: %s\n", errmsg);
        free_errmsg(errmsg);
        return false;
    }

    puts("Base de datos inicializada");

    // showData(); // Descomenta esta linea si quieres ver todos los datos guardados en la base de datos

    // Cargar usuarios, artistas, álbumes y canciones
    obtener_registros(
        "Usuarios", "*", NULL,
        cargarUsuarios, NULL, NULL);

    puts("Usuarios cargados");

    // Cargar amigos, playlists, historiales
    obtener_registros(
        "Usuarios", "id, username", NULL,
        cargarOtrosDatosPorUsuario, NULL, NULL);

    puts("Amigos, playlists e historiales cargados");

    // Cargar anuncios
    obtener_registros(
        "Anuncios JOIN Usuarios ON Usuarios.id = Anuncios.id_usuario",
        "Usuarios.username, Anuncios.url", NULL, cargarAnuncios, NULL, NULL);

    puts("Anuncios cargados");

    return true; // Datos cargados correctamente
}

bool func(FreeData)
{
    /* Aquí se libera la memoria de las estructuras de datos */

    return true;
}

/* ======== Comparadores ======== */

new_cmpfn(cmpUsuarioConUsername)
{
    const Usuario *usuario = value_ptr_1;
    const char *username = value_ptr_2;

    return strcmp(usuario->username, username);
}

new_cmpfn(cmpCancionConId)
{
    const Cancion *cancion = value_ptr_1;
    const int *id = value_ptr_2;

    return cancion->id - *id;
}
