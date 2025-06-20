#ifndef VIBECAST_DBMGR_H
#define VIBECAST_DBMGR_H

#include <VibeCastConfig.h>
#include <stdbool.h>

typedef int (*select_handler_t)(void *arg, int argc, char **argv, char **fields);
#define new_select_handler(name) int name(void *arg, int argc, char **argv, char **fields)

// Abrir base de datos
bool func(InitDB, const char *db_name, char **errmsg);

// Cerrar base de datos
void func(CloseDB);

// Función para obtener la cantidad de usuarios registrados
int userCount();

// Función para hacer un nuevo registro
bool nuevo_registro(const char *table_name, const char *table_fields, const char *values, char **errmsg);

// Función para seleccionar registros
bool obtener_registros(const char *table_name, const char *table_fields, const char *condition, select_handler_t handler, void *arg, char **errmsg);

// Función para actualizar registros
bool actualizar_registros(const char *table_name, const char *values, const char *condition, char **errmsg);

// Función para eliminar registros
bool eliminar_registros(const char *table_name, const char *condition, char **errmsg);

// Función par aliberar la memoria de un mensaje de error
void free_errmsg(char *errmsg);

#endif // VIBECAST_DBMGR_H