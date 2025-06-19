#include <db/dbmgr.h>
#include <stdio.h>

// Conexión con la base de datos
static sqlite3 *db = NULL;

// Función para crear tablas
static int create_table(const char *table_name, const char *table_fields, char **errmsg)
{
    char *sql = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS %s (%s)", table_name, table_fields);
    int rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    sqlite3_free(sql);
    return rc;
}

#define sqlerr()         \
    if (rc != SQLITE_OK) \
    return false

bool func(InitDB, const char *db_name, char **errmsg)
{
    if (db)
        return true;

    int rc;

    // Conectarse con la base de datos
    rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK)
    {
        *errmsg = sqlite3_mprintf("No se pudo establecer la conexión con la base de datos.");
        return false;
    }

    // Crear la tabla de usuarios
    rc = create_table(
        "Usuarios",
        "email TEXT NOT NULL UNIQUE, "
        "password TEXT, "
        "username TEXT PRIMARY KEY, "
        "nickname TEXT, "
        "pais TEXT, "
        "plan INTEGER, "
        "nombre_artista TEXT UNIQUE",
        errmsg);

    sqlerr();

    // Crear la tabla de amigos
    rc = create_table(
        "Amigos",
        "usuario1 TEXT, "
        "usuario2 TEXT, "
        "PRIMARY KEY(usuario1, usuario2), "
        "FOREIGN KEY(usuario1) REFERENCES Usuarios(username), "
        "FOREIGN KEY(usuario2) REFERENCES Usuarios(username)",
        errmsg);

    sqlerr();

    // Crear la tabla de álbumes
    rc = create_table(
        "Albumes",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "artista TEXT NOT NULL, "
        "nombre_album TEXT NOT NULL, "
        "fecha_creacion TEXT NOT NULL, "
        "FOREIGN KEY(artista) REFERENCES Usuarios(username)",
        errmsg);

    sqlerr();

    // Crear la tabla de canciones
    rc = create_table(
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
        "FOREIGN KEY(id_album) REFERENCES Albumes(id)",
        errmsg);

    sqlerr();

    // Crear tabla playlists
    rc = create_table(
        "Playlists",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre_playlist TEXT NOT NULL, "
        // "fecha_creacion TEXT NOT NULL, "
        "propietario TEXT NOT NULL, "
        "FOREIGN KEY(propietario) REFERENCES Usuarios(username)",
        errmsg);

    sqlerr();

    // Crear la tabla para las canciones por playlist
    rc = create_table(
        "Playlist_Canciones",
        "id_playlist INTEGER NOT NULL, "
        "id_cancion INTEGER NOT NULL, "
        "PRIMARY KEY(id_playlist, id_cancion), "
        "FOREIGN KEY(id_playlist) REFERENCES Playlists(id), "
        "FOREIGN KEY(id_cancion) REFERENCES Canciones(id)",
        errmsg);

    sqlerr();

    // Crear la tabla para las reproducciones
    rc = create_table(
        "Reproducciones",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL, "
        "id_cancion INTEGER NOT NULL, "
        "fecha_escuchado TEXT NOT NULL, "
        "FOREIGN KEY(username) REFERENCES Usuarios(username), "
        "FOREIGN KEY(id_cancion) REFERENCES Canciones(id)",
        errmsg);

    sqlerr();

    // Crear la tabla para los historiales
    rc = create_table(
        "Historiales",
        "username TEXT PRIMARY KEY, "
        "tiempo_escuchado REAL DEFAULT 0.0, "
        "cantidad_anuncios INTEGER DEFAULT 0, "
        "FOREIGN KEY(username) REFERENCES Usuarios(username)",
        errmsg);

    sqlerr();

    // Crear la tabla para los anuncios publicitarios
    rc = create_table(
        "Anuncios",
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "anunciante TEXT NOT NULL, "
        "url TEXT NOT NULL, "
        "FOREIGN KEY(anunciante) REFERENCES Usuarios(username)",
        errmsg);

    sqlerr();

    return true;
}

void func(CloseDB)
{
    if (db)
    {
        sqlite3_close(db);
        db = NULL;
    }
}

static new_select_handler(getCount)
{
    sscanf(*argv, "%d", (int *)arg);
    return SQLITE_OK;
}

int contar_registros(const char *table_name, char **errmsg)
{
    int count = 0;

    char *sql = sqlite3_mprintf(R("COUNT(*)", "%s"), table_name);

    sqlite3_exec(db, R("COUNT(*)", "Usuarios"), getCount, &count, NULL);

    return count;
}

bool nuevo_registro(const char *table_name, const char *table_fields, const char *values, char **errmsg)
{
    char *sql = sqlite3_mprintf(C("%s", "%s", %s), table_name, table_fields, values);
    if (!sql)
        return false;
    int rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    sqlite3_free(sql);
    return rc == SQLITE_OK;
}

bool obtener_registros(const char *table_name, const char *table_fields, const char *condition, select_handler_t handler, void *arg, char **errmsg)
{
    char *sql = NULL;
    if (condition)
    {
        sql = sqlite3_mprintf(R("%s", "%s", WHERE %s), table_name, table_fields, condition);
    }
    else
    {
        sql = sqlite3_mprintf(R("%s", "%s"), table_fields, table_name);
    }

    if (!sql)
        return false;
    int rc = sqlite3_exec(db, sql, handler, arg, errmsg);

    sqlite3_free(sql);

    return rc == SQLITE_OK;
}

bool actualizar_registros(const char *table_name, const char *values, const char *condition, char **errmsg)
{
    char *sql = sqlite3_mprintf(U("%s", "%s", %s), table_name, condition, values);

    if (!sql)
        return false;
    int rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    
    sqlite3_free(sql);
    
    return rc == SQLITE_OK;
}

bool eliminar_registros(const char *table_name, const char *condition, char **errmsg)
{
    char *sql = sqlite3_mprintf(D("%s", "%s"), table_name, condition);

    if (!sql)
        return false;
    int rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);

    sqlite3_free(sql);
    
    return rc == SQLITE_OK;
}

void free_errmsg(char *errmsg)
{
    sqlite3_free(errmsg);
}
