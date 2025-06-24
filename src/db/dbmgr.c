#include <db/dbmgr.h>
#include <utils/utils.h>
#include <sqlite3.h>
#include <stdio.h>

// Conexión con la base de datos
static sqlite3 *db = NULL;

bool func(InitDB, const char *db_name, const char *script_filename, char **errmsg)
{
    if (db)
        return true;

    if (!script_filename)
        return false;

    // Conectarse con la base de datos
    int rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK)
    {
        if (errmsg)
            *errmsg = sqlite3_mprintf("No se pudo establecer la conexión con la base de datos.");
        return false;
    }

    // Cargar el script

    // Abrir el archivo
    FILE *script_file = fopen(script_filename, "r");
    if (!script_file)
    {
        if (errmsg)
            *errmsg = sqlite3_mprintf("Error al abrir el archivo de script.");
        return false;
    }

    // Obtener el tamaño del archivo
    fseek(script_file, 0, SEEK_END);
    int script_len = ftell(script_file);

    // Asignar memoria para cargar el script
    char *script_str = malloc_cpy(script_len + 1, NULL);
    if (!script_str)
    {
        if (errmsg)
            *errmsg = sqlite3_mprintf("Error al cargar el script.");
        fclose(script_file);
        return false;
    }

    rewind(script_file);                           // Regresar al inicio del archivo
    fread(script_str, 1, script_len, script_file); // Copiar el script
    fclose(script_file);                           // Cerrar el archivo
    script_str[script_len] = '\0';                 // Asegurar que el string termine en '\0'

    // Ejecutar el script
    rc = sqlite3_exec(db, script_str, NULL, NULL, errmsg);
    free(script_str); // Liberar la memoria del script

    return rc == SQLITE_OK;
}

void func(CloseDB)
{
    if (db && (sqlite3_close(db) == SQLITE_OK))
        db = NULL;

    if (!db)
        puts("Base de datos cerrada");
}

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

bool ejecutar_script(const char *script, select_handler_t handler, void *arg, char **errmsg)
{
    return script && (sqlite3_exec(db, script, handler, arg, errmsg) == SQLITE_OK);
}

void free_errmsg(char *errmsg)
{
    sqlite3_free(errmsg);
}
