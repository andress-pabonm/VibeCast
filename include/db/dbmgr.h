#ifndef VIBECAST_DBMGR_H
#define VIBECAST_DBMGR_H

#include <sqlite3.h>

/**
 * 1. Inicializar base de datos (establecer la conexión con la base de datos)
 * 2. Cerrar la conexión con la base de datos
 *
 * 3. Función para cargar todos los usuarios
 * 4. Función para cargar todos los artistas
 * 5. Función para cargar todos los albumes
 * 6. Función para cargar todas las canciones
 * 
 * Nota: Estaba pensando en cómo cargar el historial,
 * aún no tengo una idea clara, tendría que entender
 * cómo funciona la base de datos
 */

void VibeCast_InitDB();  // Abrir base de datos
void VibeCast_CloseDB(); // Cerrar base de datos

/* Estas funciones idealmente cargarían los datos en la variable (datos) y devolverían la cantidad de registros */
size_t VibeCast_CargarUsuarios(void **datos);
size_t VibeCast_CargarArtistas(void **datos);
size_t VibeCast_CargarAlbumes(void **datos);
size_t VibeCast_CargarCanciones(void **datos);

#endif // VIBECAST_DBMGR_H