#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_WindowFlags flags = (SDL_WINDOW_MAXIMIZED);

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("VibeCast", 800, 600, flags, &window, &renderer))
    {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    puts("Inicialización completa");
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const char *message = "Hello World!";
    int w = 0, h = 0;
    float x, y;
    const float scale = 1.0f;

    /* Center the message and scale it up */
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, scale, scale);
    x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
    y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);       // Cambiar el color de dibujado a negro
    SDL_RenderClear(renderer);                            // Limpiar el lienzo con el color previamente elegido
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Cambiar el color de dibujado a blanco
    SDL_RenderDebugText(renderer, x, y, message);         // Renderizar el texto
    SDL_RenderPresent(renderer);                          // Mostrar la imagen renderizada

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
