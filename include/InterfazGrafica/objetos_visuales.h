#ifndef VIBECAST_OBJETOS_VISUALES_H
#define VIBECAST_OBJETOS_VISUALES_H

#include <SDL3/SDL.h>
#include <InterfazGrafica/colores.h>
typedef enum
{
    TEXT_ALIGN_LEFT,   // Alinear texto a la izquierda
    TEXT_ALIGN_CENTER, // Alinear texto al centro
    TEXT_ALIGN_RIGHT   // Alinear texto a la derecha
} TextAlign;
typedef struct
{
    SDL_FRect cuadro; // Posición y tamaño del objeto
    Uint8 padding;    // Separación del texto con el borde

    SDL_Color bg_color; // Color del fondo
    SDL_Color fg_color; // Color del contenido
    SDL_Color bd_color; // Color del borde

    TextAlign text_align; // Alineación del texto
    char *text;           // Texto del objeto
} ObjetoVisual;

typedef ObjetoVisual CampoTexto; // Para entrada de texto
typedef ObjetoVisual Boton;      // Un simple botón
typedef ObjetoVisual Texto;      // Un texto plano

#endif // VIBECAST_OBJETOS_VISUALES_H