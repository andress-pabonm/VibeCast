#ifndef VIBECAST_OBJETOS_VISUALES_H
#define VIBECAST_OBJETOS_VISUALES_H

#include <SDL3/SDL.h>

typedef struct
{
    SDL_Rect cuadro;
    char *texto;
} ObjetoVisual;

typedef ObjetoVisual CampoTexto;
typedef ObjetoVisual Boton;
typedef ObjetoVisual Texto;

// Funciones para renderizar estos objetos visuales

void VibeCast_RenderCampoTexto(const CampoTexto *campoTexto); // Renderizar un campo de texto.
void VibeCast_RenderBoton(const Boton *boton);                // Renderizar un botón.
void VibeCast_RenderTexto(const Texto *texto);                // Renderizar texto.

#endif // VIBECAST_OBJETOS_VISUALES_H