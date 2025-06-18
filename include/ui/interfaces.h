#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

#include <VibeCastConfig.h>
#include <db/datos.h>

#include <stdbool.h>

typedef enum
{
    LOGIN
} Interfaz;

/**
 * Macro para facilitar la llamada a los archivos .html
 * Utilizas la ruta relativa al directorio ui donde están las .html
 * Uso: INTERFAZ("carpeta/archivo.html")
 */
#define INTERFAZ(nombre) "file://" path_to("ui/" nombre)

/* Aquí van las declaraciones de las funciones principales de cada interfaz */

bool iniciar_sesion(const char *data);
bool crear_cuenta(const char *data);
bool cerrar_sesion();

#endif // VIBECAST_INTERFACES_H