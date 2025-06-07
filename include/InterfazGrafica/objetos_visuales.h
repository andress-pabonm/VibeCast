#ifndef VIBECAST_OBJETOS_VISUALES_H
#define VIBECAST_OBJETOS_VISUALES_H

#include <SDL3/SDL.h>
#include <InterfazGrafica/colores.h>
typedef struct
{
    SDL_FRect cuadro; // Posición y tamaño del objeto
    Uint8 border;     // Ancho del borde

    SDL_Color bg_color; // Color del fondo
    SDL_Color fg_color; // Color del contenido
    SDL_Color bd_color; // Color del borde

    char *text; // Texto del objeto
} ObjetoVisual;

typedef ObjetoVisual CampoTexto; // Para entrada de texto
typedef ObjetoVisual Boton;      // Un simple botón
typedef ObjetoVisual Texto;      // Un texto plano

#endif // VIBECAST_OBJETOS_VISUALES_H