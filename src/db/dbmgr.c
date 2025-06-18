#include <db/dbmgr.h>
#include <VibeCastConfig.h>
#include <sqlite3.h>

// Conexión con la base de datos
static sqlite3 *db = NULL;

static void create_table(const char *table_name, const char *table_fields)
{
    char *sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS %s (%s)", table_name, table_fields);
    if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK)
    {
        printf("No se pudo crear la tabla: %s\n", table_name);
    }
    sqlite3_free(sql);
}

void func(InitDB)
{
    if (db)
        return;

    // Conectarse con la base de datos
    sqlite3_open(":memory:", &db);

    // Crear la tabla de usuarios
    create_table(
        "Usuarios",
        "email TEXT NOT NULL UNIQUE, "
        "password TEXT, "
        "username TEXT PRIMARY KEY, "
        "nickname TEXT, "
        "pais TEXT, "
        "plan INTEGER, "
        "nombre_artista TEXT UNIQUE");

    // Crear la tabla de amigos
    create_table(
        "Amigos",
        "usuario1 TEXT, "
        "usuario2 TEXT, "
        "PRIMARY KEY(usuario1, usuario2), "
        "FOREIGN KEY(usuario1) REFERENCES Usuarios(username), "
        "FOREIGN KEY(usuario2) REFERENCES Usuarios(username)");

    // Crear la tabla de álbumes
    create_table(
        "Albumes",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "artista TEXT NOT NULL, "
        "nombre_album TEXT NOT NULL, "
        "fecha_creacion TEXT NOT NULL, "
        "FOREIGN KEY(artista) REFERENCES Usuarios(username)");

    // Crear la tabla de canciones
    create_table(
        "Canciones",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "id_album INTEGER NOT NULL, "
        "nombre_cancion TEXT NOT NULL, "
        "genero TEXT NOT NULL, "
        "fecha_lanzamiento TEXT NOT NULL, "
        "popularidad INTEGER DEFAULT 0, "
        "reproducciones INTEGER DEFAULT 0, "
        "duracion REAL NOT NULL, "
        "url TEXT NOT NULL, "
        "FOREIGN KEY(id_album) REFERENCES Albumes(id)");

    // Crear tabla playlists
    create_table(
        "Playlists",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_playlist TEXT NOT NULL, "
        // "fecha_creacion TEXT NOT NULL, "
        "propietario TEXT NOT NULL, "
        "FOREIGN KEY(propietario) REFERENCES Usuarios(username)");

    // Crear la tabla para las canciones por playlist
    create_table(
        "Playlist_Canciones",
        "id_playlist INTEGER NOT NULL, "
        "id_cancion INTEGER NOT NULL, "
        "PRIMARY KEY(id_playlist, id_cancion), "
        "FOREIGN KEY(id_playlist) REFERENCES Playlists(id), "
        "FOREIGN KEY(id_cancion) REFERENCES Canciones(id)");

    // Crear la tabla para las reproducciones
    create_table(
        "Reproducciones",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL, "
        "id_cancion INTEGER NOT NULL, "
        "fecha TEXT NOT NULL, "
        "FOREIGN KEY(username) REFERENCES Usuarios(username), "
        "FOREIGN KEY(id_cancion) REFERENCES Canciones(id)");

    // Crear la tabla para los historiales
    create_table(
        "Historiales",
        "username TEXT PRIMARY KEY, "
        "tiempo_escuchado REAL DEFAULT 0.0, "
        "cantidad_anuncios INTEGER DEFAULT 0, "
        "FOREIGN KEY(username) REFERENCES Usuarios(username)");

    // Crear la tabla para los anuncios publicitarios
    create_table(
        "Anuncios",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "anunciante TEXT NOT NULL, "
        "url TEXT NOT NULL, "
        "FOREIGN KEY(anunciante) REFERENCES Usuarios(username)");
}

void func(CloseDB)
{
    if (db)
    {
        sqlite3_close(db);
        db = NULL;
    }
}
