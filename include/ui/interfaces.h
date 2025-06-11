#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

#include <SDL3/SDL.h>

/* Este es un tipo personalizado para hacer referencia a las diferentes interfaces */
typedef enum
{
    LOGIN
} VibeCast_InterfazID;

/* Aquí van las declaraciones de las funciones principales de cada interfaz */

SDL_AppResult VibeCast_Login(void *appstate);

#endif // VIBECAST_INTERFACES_H