#ifndef VIBECAST_DBMGR_H
#define VIBECAST_DBMGR_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>

/* ======== Función para las búsquedas ======== */

/**
 * Tipo para funciones para operar sobre los registros obtenidos en una búsqueda en la base de datos.
 * @param arg: Argumento provisto por el desarrollador.
 * @param argc: Cantidad de campos obtenidos por registro.
 * @param argv: Valores de los campos obtenidos por registro.
 * @param fields: Nombres de los campos obtenidos.
 * @return Retorna 0 si debe continuar con los demás registros, caso contrario retorna un número diferente a 0
 */
typedef int (*select_handler_t)(void *arg, int argc, char **argv, char **fields);

// Para declarar y definir fácilmente funciones de tipo (select_handler_t)
#define select_handler(name) int name(void *arg, int argc, char **argv, char **fields)

/* ======== Funciones para la conexión con la base de datos ======== */

// Abrir base de datos
bool func(InitDB, const char *db_name, const char *script_filename, char **errmsg);

// Cerrar base de datos
void func(CloseDB);

// Ejecutar un script sobre la base de datos abierta
bool func(RunScript, const char *script_filename, char **errmsg);

/* ======== Funciones para las operaciones básicas sobre la base de datos ======== */

// Para contar la cantidad de registros en una tabla que cumplen una condición.
int contar_registros(const char *table_name, const char *condition, char **errmsg);

// Función para hacer un nuevo registro
bool nuevo_registro(const char *table_name, const char *table_fields, const char *values, char **errmsg);

// Función para seleccionar registros
bool obtener_registros(const char *table_name, const char *table_fields, const char *condition, select_handler_t handler, void *arg, char **errmsg);

// Función para actualizar registros
bool actualizar_registros(const char *table_name, const char *values, const char *condition, char **errmsg);

// Función para eliminar registros
bool eliminar_registros(const char *table_name, const char *condition, char **errmsg);

/* ======== Wrappers de funciones de SQLite ======== */

// Función par aliberar la memoria de un mensaje de error
void free_errmsg(char *errmsg);

#endif // VIBECAST_DBMGR_H