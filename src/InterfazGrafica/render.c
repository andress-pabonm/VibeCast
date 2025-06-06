#include <InterfazGrafica/render.h>
#include <InterfazGrafica/objetos_visuales.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

bool VibeCast_InitVideo()
{
    if (renderer)
        return true;

    SDL_WindowFlags flags = (SDL_WINDOW_MAXIMIZED);

    return SDL_CreateWindowAndRenderer("VibeCast", 800, 600, flags, &window, &renderer);
}

void VibeCast_ClearScreen(SDL_Color color)
{
    if (!renderer)
        return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void VibeCast_ShowScreen()
{
    if (!renderer)
        return;

    SDL_RenderPresent(renderer);
}

void VibeCast_DestroyVideo()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    /* También se destruirían otros recursos iniciados en este archivo */
}

// Funciones para renderizar elementos visuales

void VibeCast_RenderCampoTexto(const CampoTexto *campoTexto)
{
    if (!campoTexto)
        return;
}

void VibeCast_RenderBoton(const Boton *boton)
{
    if (!boton)
        return;
}

void VibeCast_RenderTexto(const Texto *texto)
{
    if (!texto)
        return;

    int w = 0, h = 0;
    float x, y;
    const float scale = 1.0f;

    /* Center the message and scale it up */
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, scale, scale);
    x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(texto->texto)) / 2;
    y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, x, y, texto->texto);
}
