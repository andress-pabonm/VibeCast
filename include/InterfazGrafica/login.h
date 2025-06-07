#ifndef VIBECAST_LOGIN_H
#define VIBECAST_LOGIN_H

#include <SDL3/SDL.h>
#include <InterfazGrafica/interfaces.h>
#include <InterfazGrafica/render.h>

SDL_AppResult VibeCast_LoginEventHandler(InterfazID *interfaz, SDL_Event *event);
SDL_AppResult VibeCast_Login(InterfazID *interfaz);

#endif // VIBECAST_LOGIN_H