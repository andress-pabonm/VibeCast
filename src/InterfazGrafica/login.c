#include <InterfazGrafica/login.h>
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

    /* Por ejemplo, para mostrar un texto centrado */

    SDL_asprintf(&(texto.text), "¡Hola mundo!");

    texto.cuadro.w = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(texto.text);
    texto.cuadro.h = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

    texto.border = 0;

    texto.bg_color = COLOR(NEGRO, TRANSPARENTE); // Color de fondo
    texto.fg_color = COLOR(BLANCO, OPACO);       // Color del texto
    texto.bd_color = texto.bg_color;             // Color del borde

    initialized = true; // Se ha inicializado el login
}

/* Función para actualizar los objetos de la ventana login */
void update()
{
    /* Por ejemplo, puedo decir que el botón vaya cambiando su tamaño según el tamaño de la ventana */
    /* Cambiar tamaño botones según tamaño ventana */

    int w, h; // Para el tamaño de la ventana

    VibeCast_GetRenderSize(&w, &h);

    texto.cuadro.x = (w - texto.cuadro.w) / 2;
    texto.cuadro.y = (h - texto.cuadro.h) / 2;
}

/* En esta función se renderizan los elementos */
void render()
{
    /* Dibujar elementos visuales */
    /* Mostrar/renderizar los objetos visuales */
    // VibeCast_RenderButton(&iniciarSesion);

    VibeCast_ClearScreen(COLOR(NEGRO, OPACO)); // Limpiar la pantalla con el color negro

    VibeCast_Render(&texto); // Dibujo el texto

    VibeCast_ShowScreen(); // Mostrar los cambios
}

SDL_AppResult VibeCast_LoginEventHandler(InterfazID *interfaz, SDL_Event *event)
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
