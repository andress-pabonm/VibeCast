#define SDL_MAIN_USE_CALLBACKS 1 /* Usar los callbacks en lugar de main() */

/* Incluye las librerias de SDL */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // Para los callbacks

/* Para inicializar las interfaces de audio y video */
#include <InterfazGrafica/render.h>
#include <Audio/player.h>

/* Incluir las interfaces */
#include <InterfazGrafica/login.h>
// #include <InterfazGrafica/menu.h>

#include <InterfazGrafica/interfaces.h>
InterfazID interfaz;

/* Esta función se ejecuta una vez al iniciar */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Inicializa el subsitema de video y audio */
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("No se puede inicilizar el sistema de video o audio: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Inicializa la ventana y el rederizador */
    if (!VibeCast_InitVideo())
    {
        SDL_Log("No se puede inicilziar la ventana o el render: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Abre el flujo de datos para el dispositivo de salida por defecto */
    if (!VibeCast_InitAudio())
    {
        SDL_Log("Couldn't open audio device stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    interfaz = LOGIN; // Iniciar con el login

    return SDL_APP_CONTINUE;
}

/* Esta función es llamada cuando ocurre un nuevo evento (mouse, teclado, etc) */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS; // Se cerró la ventana

    switch (interfaz)
    {
    case LOGIN:
        return VibeCast_LoginEventHandler(event);
    // case MENU:
    //     return VibeCast_MenuEventHandler(event);
    }

    return SDL_APP_CONTINUE;
}

/* Esta función se ejecuta una vez por cuadro. Es el bucle principal del programa */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    switch (interfaz)
    {
    case LOGIN:
        return VibeCast_Login(&interfaz);
    }

    return SDL_APP_CONTINUE;
}

/* Esta función se llama una vez al finalizar la ejecución */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // VibeCast_DestroyVideo();
    // VibeCast_DestroyAudio();
}
