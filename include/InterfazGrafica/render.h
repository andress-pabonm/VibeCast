#ifndef VIBECAST_RENDER_H
#define VIBECAST_RENDER_H

#include <SDL3/SDL.h>
#include <stdbool.h>

bool VibeCast_InitVideo();                  // Inicializar las variables para ventana y renderizados
void VibeCast_ClearScreen(SDL_Color color); // Limpiar el lienzo
void VibeCast_ShowScreen();                 // Mostrar el lienzo
void VibeCast_DestroyVideo();               // Para liberar la memoria de los recursos visuales utilizados

#endif // VIBECAST_RENDER_H