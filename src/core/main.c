#define SDL_MAIN_USE_CALLBACKS 1 /* Usar los callbacks en lugar de main() */

/* Incluye las librerias de SDL */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // Para los callbacks

#include <stdio.h>

#include <utils/utils.h>

#include <ui/interfaces.h>
#include <ui/login.h>

/* Esta función se ejecuta una vez al iniciar */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_InitFlags flags = SDL_INIT_AUDIO | SDL_INIT_VIDEO;

    if (!SDL_Init(flags)) {
        SDL_Log("Error: No se pudo inicializar SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Abrir dispositivo de audio */
    /* Abrir dispositivo de video */

    VibeCast_InterfazID *id = alloc(VibeCast_InterfazID, NULL);
    *id = LOGIN;
    *appstate = id; // Para pasar el id de la interfaz a las otras funciones

    return SDL_APP_CONTINUE;
}

/* Esta función es llamada cuando ocurre un nuevo evento (mouse, teclado, etc) */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS; // Se cerró la ventana

    switch (*cast(VibeCast_InterfazID *, appstate))
    {
    case LOGIN:
        puts("Login event");
        break;

    default:
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* Esta función se ejecuta una vez por cuadro. Es el bucle principal del programa */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    switch (*cast(VibeCast_InterfazID *, appstate))
    {
    case LOGIN:
        return VibeCast_LoginUI(appstate);

    default:
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* Esta función se llama una vez al finalizar la ejecución */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_free(appstate);
}
