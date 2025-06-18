#ifndef VIBECAST_DBMGR_H
#define VIBECAST_DBMGR_H

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

#endif // VIBECAST_DBMGR_H