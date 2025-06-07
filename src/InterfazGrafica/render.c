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

void VibeCast_GetRenderSize(int *w, int *h)
{
    SDL_GetRenderOutputSize(renderer, w, h);
}

void VibeCast_SetRenderScale(float x_scale, float y_scale)
{
    SDL_SetRenderScale(renderer, x_scale, y_scale);
}

/* Función para denderizad/dibujar un objeto visual */
void VibeCast_Render(ObjetoVisual *obj)
{
    if (!obj)
        return; // No hay objeto

    /* Dibujo el fondo */
    if (obj->cuadro.w && obj->cuadro.h)
    {
        SDL_SetRenderDrawColor(renderer, obj->bg_color.r, obj->bg_color.g, obj->bg_color.b, obj->bg_color.a);
        SDL_RenderFillRect(renderer, &obj->cuadro);
    }

    /* Dibujo el borde */
    if (obj->border)
    {
        SDL_FRect borde = obj->cuadro;
        SDL_SetRenderDrawColor(renderer, obj->bd_color.r, obj->bd_color.g, obj->bd_color.b, obj->bd_color.a);
        for (int i = 0; i < obj->border; i++)
        {
            SDL_RenderRect(renderer, &borde);

            /* Hacer más pequeño el cuadro del borde */
            borde.x += 1;
            borde.y += 1;
            borde.w -= 2;
            borde.h -= 2;
        }
    }

    /* Dibujo el texto */
    if (obj->text)
    {
        SDL_SetRenderDrawColor(renderer, obj->fg_color.r, obj->fg_color.g, obj->fg_color.b, obj->fg_color.a);
        SDL_RenderDebugText(renderer, obj->cuadro.x, obj->cuadro.y, obj->text);
    }
}
