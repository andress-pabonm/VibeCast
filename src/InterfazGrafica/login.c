#include <InterfazGrafica/login.h>
#include <InterfazGrafica/render.h>
#include <InterfazGrafica/objetos_visuales.h>
#include <stdbool.h>

/* Para saber si está inicializado */
static bool initialized = false;

/* Aquí coloco los objetos visuales que tendrá mi login */
static Texto texto;

/* Aquí inicializo los elementos de mi login */
void init()
{
    if (initialized)
        return; // Ya se inicializó el login

    /* Por ejemplo */
    texto.cuadro = (SDL_Rect){0, 0, 0, 0}; // Ignorado por ahora
    SDL_asprintf(&(texto.texto), "%s", "¡Hola mundo!");

    initialized = true; // Se ha inicializado el login
}

/* Función para actualizar los objetos de la ventana login */
void update()
{
    /* Por ejemplo, puedo decir que el botón vaya cambiando su tamaño según el tamaño de la ventana */
    /* Cambiar tamaño botones según tamaño ventana */
}

/* En esta función se renderizan los elementos */
void render()
{
    /* Dibujar elementos visuales */
    /* Mostrar/renderizar los objetos visuales */
    // VibeCast_RenderButton(&iniciarSesion);

    VibeCast_ClearScreen((SDL_Color){0, 0, 0, 255}); // Negro

    VibeCast_RenderTexto(&texto);

    VibeCast_ShowScreen(); // Mostrar los cambios
}

SDL_AppResult VibeCast_LoginEventHandler(SDL_Event *event)
{
    /* En esta función se manejaría los eventos para el login */
    /* Verificar si pulsó un boton del login */

    return SDL_APP_CONTINUE;
}

/* Función que se llamará para mostrar la ventana de login */
/* También puede incluir la lógica que involucra el login */
SDL_AppResult VibeCast_Login(InterfazID *interfaz)
{
    if (!initialized)
        init(); // Inicializar el login

    update(); // Actualizar lo que haya que actualizar
    render(); // Dibujar lo que haya que dibujar

    return SDL_APP_CONTINUE;
}
