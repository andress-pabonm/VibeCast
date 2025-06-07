#ifndef COLORES_H
#define COLORES_H

#include <SDL3/SDL.h>
#include <Utilidades/utils.h>

/* Opacidad */
#define OPACO 255
#define TRANSPARENTE 0

/* Color */
#define BLANCO 255, 255, 255
#define NEGRO 0, 0, 0
#define ROJO 255, 0, 0
#define AMARILLO 255, 255, 0
#define VERDE 0, 255, 0
#define CIAN 0, 255, 255
#define AZUL 0, 0, 255
#define MAGENTA 255, 0, 255

/* Ejemplo: (SDL_Color) {AZUL, OPACO} */

#define COLOR(color, opacidad) cast(SDL_Color, color, opacidad)

#endif // COLORES_H