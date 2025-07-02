#include <db/dbmgr.h>
#include <utils/utils.h>
#include <sqlite3.h>
#include <stdio.h>
#include <io.h>
#include <errno.h>

// Conexión con la base de datos
static sqlite3 *db = NULL;

bool VibeCast_InitDB(const char *db_name, char **errmsg)
{
    if (db)
        return true;

    // Verificar si el archivo ya existe
    bool db_existed = (_access(db_name, 0) == 0);

    // Conectarse con la base de datos
    int rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK)
    {
        if (errmsg)
            *errmsg = sqlite3_mprintf("No se pudo establecer la conexión con la base de datos.");
        return false;
    }

    // Si la base ya existía, no ejecutar el script
    if (db_existed)
        return true;

    return VibeCast_RunScript("db_setup.sql", errmsg) &&
           VibeCast_RunScript("datos_prueba.sql", errmsg);
}

void VibeCast_CloseDB()
{
    if (db && (sqlite3_close(db) == SQLITE_OK))
        db = NULL;
}

bool VibeCast_RunScript(const char *filename, char **errmsg)
{
    if (!db)
    {
        if (errmsg)
            *errmsg = sqlite3_mprintf("Base de datos no abierta.");
        return false;
    }

    char *buf = readFile(filename, errmsg);
    if (!buf)
        return false;

    int rc = sqlite3_exec(db, buf, NULL, NULL, errmsg);
    freem(buf);

    return rc == SQLITE_OK;
}

/* ======== Funciones para interactuar con la base de datos ======== */

static select_handler(getCount)
{
    sscanf(*argv, "%d", (int *)arg);
    return SQLITE_OK;
}

int contar_registros(const char *table_name, const char *condition, char **errmsg)
{
    int count = 0;
    obtener_registros(table_name, "COUNT(*)", condition, getCount, &count, errmsg);
    return count;
}

bool nuevo_registro(const char *table_name, const char *table_fields, const char *values, char **errmsg)
{
    char *sql = sqlite3_mprintf("INSERT INTO %s (%s) VALUES (%s)", table_name, table_fields, values);
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
        sql = sqlite3_mprintf("SELECT %s FROM %s WHERE %s", table_fields, table_name, condition);
    else
        sql = sqlite3_mprintf("SELECT %s FROM %s", table_fields, table_name);

    if (!sql)
        return false;
    int rc = sqlite3_exec(db, sql, handler, arg, errmsg);
    sqlite3_free(sql);

    return rc == SQLITE_OK;
}

bool actualizar_registros(const char *table_name, const char *values, const char *condition, char **errmsg)
{
    char *sql = sqlite3_mprintf("UPDATE %s SET %s WHERE %s", table_name, values, condition);

    if (!sql)
        return false;
    int rc = sqlite3_exec(db, sql, NULL, NULL, errmsg);
    sqlite3_free(sql);

    return rc == SQLITE_OK;
}

bool eliminar_registros(const char *table_name, const char *condition, char **errmsg)
{
    char *sql = sqlite3_mprintf("DELETE FROM %s WHERE %s", table_name, condition);

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
