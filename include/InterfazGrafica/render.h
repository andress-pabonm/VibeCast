#ifndef VIBECAST_RENDER_H
#define VIBECAST_RENDER_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <InterfazGrafica/objetos_visuales.h>

bool VibeCast_InitVideo();                                  // Inicializar las variables para ventana y renderizados
void VibeCast_ClearScreen(const SDL_Color color);           // Limpiar el lienzo
void VibeCast_ShowScreen();                                 // Mostrar el lienzo
void VibeCast_DestroyVideo();                               // Para liberar la memoria de los recursos visuales utilizados
void VibeCast_GetRenderSize(int *w, int *h);                // Para obtener el tamaño del lienzo de dibujo
void VibeCast_SetRenderScale(float x_scale, float y_scale); // Para establecer la escala de dibujado
void VibeCast_Render(ObjetoVisual *obj);                    // Para dibujar un objeto visual dado

#endif // VIBECAST_RENDER_H