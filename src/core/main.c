#define SDL_MAIN_USE_CALLBACKS 1 /* Usar los callbacks en lugar de main() */

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // Para los callbacks

/* Variables utilizadas para el video */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* Variables utilizadas para el audio */
static SDL_AudioStream *stream = NULL; // Flujo de salida para audio (como stdout)
static Uint8 *wav_data = NULL;         // Para guardar la data de un WAV
static Uint32 data_len = 0;            // Longitud de la data de un WAV

/* Esta función se ejecuta una vez al iniciar */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Inicializa el subsitema de video y audio */
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_WindowFlags flags = (SDL_WINDOW_MAXIMIZED);

    /* Crea una ventana y su renderizador */
    if (!SDL_CreateWindowAndRenderer("VibeCast", 800, 600, flags, &window, &renderer))
    {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Abre el flujo de datos para el dispositivo de salida por defecto */
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);
    if (!stream)
    {
        SDL_Log("Couldn't open audio device stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* Esta función es llamada cuando ocurre un nuevo evento (mouse, teclado, etc) */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:        // Al cerrar la ventana
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }

    return SDL_APP_CONTINUE;
}

/* Esta función se ejecuta una vez por cuadro. Es el bucle principal del programa */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    const char *message = "Hello World!";
    int w = 0, h = 0;
    float x, y;
    const float scale = 4.0f;

    /* Centra el texto y lo escala */
    SDL_GetRenderOutputSize(renderer, &w, &h);                                        // Tamaño del lienzo de dibujo
    SDL_SetRenderScale(renderer, scale, scale);                                       // Establecer la escala a la que se dibujará
    x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2; // Centrar para X
    y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;                       // Centrar para Y

    /* Limpiar el lienzo */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Cambiar el color de dibujado a negro
    SDL_RenderClear(renderer);                      // Limpiar el lienzo con el color previamente elegido

    /* Dibujar el mensaje */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Cambiar el color de dibujado a blanco
    SDL_RenderDebugText(renderer, x, y, message);         // Renderizar el texto

    /* Mostrar los cambios en el lienzo */
    SDL_RenderPresent(renderer); // Mostrar la imagen renderizada

    return SDL_APP_CONTINUE;
}

/* Esta función se llama una vez al finalizar la ejecución */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}
