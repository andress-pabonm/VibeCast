#include <BaseDatos/dbmgr.h>

static sqlite3 *base_datos = NULL; // Conexión con la base de datos

void VibeCast_InitDB()
{
    if (!base_datos)                           // Si no hay una conexión con la base de datos
        sqlite3_open("datos.db", &base_datos); // Conéctate a la base de datos
}

void VibeCast_CloseDB()
{
    if (base_datos) // Si hay una conexión abierta a la base de datos
    {
        sqlite3_close(base_datos); // Ciérrala
        base_datos = NULL;         // Y define la variable como NULL
    }
}

size_t VibeCast_CargarUsuarios(void **datos)
{
    if (!base_datos) // Si no hay una conexión con la base de datos
        return 0;    // No se pueden cargar los datos

    /* Código para cargar los usuarios */
    return 0;
}

size_t VibeCast_CargarArtistas(void **datos)
{
    if (!base_datos) // Si no hay una conexión con la base de datos
        return 0;    // No se pueden cargar los datos

    /* Código para cargar los artistas */
    return 0;
}

size_t VibeCast_CargarAlbumes(void **datos)
{
    if (!base_datos) // Si no hay una conexión con la base de datos
        return 0;    // No se pueden cargar los datos

    /* Código para cargar los álbumes */
    return 0;
}

size_t VibeCast_CargarCanciones(void **datos)
{
    if (!base_datos) // Si no hay una conexión con la base de datos
        return 0;    // No se pueden cargar los datos

    /* Código para cargar las canciones */
    return 0;
}
