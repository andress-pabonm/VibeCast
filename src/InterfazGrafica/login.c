#include <InterfazGrafica/login.h>
#include <stdbool.h>

/* Para saber si está inicializado */
static bool initialized = false;

/* ------------------------------------------------ */

// Colores utilizados

/* Para el fondo de la ventana */
#define BG_COLOR 0x1E, 0x1E, 0x2F

/* Botón "Iniciar sesión" */
#define BG_COLOR_1 0x2D, 0x9C, 0xDB
#define BD_COLOR_1 0x56, 0xCC, 0xF2 // azul más suave
#define FG_COLOR_1 0x00, 0x2B, 0x3D

/* Botón "Crear cuenta" */
#define BG_COLOR_2 0x27, 0xAE, 0x60
#define BD_COLOR_2 0x6F, 0xCF, 0x97 // verde claro
#define FG_COLOR_2 0x00, 0x39, 0x1C

/* Botón "Continuar como invitado" */
#define BG_COLOR_3 0xF2, 0x99, 0x4A
#define BD_COLOR_3 0xF2, 0xC9, 0x4C // amarillo dorado
#define FG_COLOR_3 0x4A, 0x29, 0x00

/* Aquí coloco los objetos visuales que tendrá mi login */
static Boton btnIniciarSesion;
#define TEXTO_INICIAR_SESION "Iniciar sesión"

static Boton btnCrearCuenta;
#define TEXTO_CREAR_CUENTA "Crear cuenta"

static Boton btnInvitado;
#define TEXTO_INVITADO "Continuar como invitado"

/* ------------------------------------------------ */

/* Aquí inicializo los elementos de mi login */
void init()
{
    if (initialized)
        return; // Ya se inicializó el login

    int padding = 20;
    int ancho_texto = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(TEXTO_INVITADO);
    int alto_texto = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
    int ancho_botones = 2 * padding + ancho_texto;
    int alto_botones = 2 * padding + alto_texto;

    // Primer botón: Iniciar sesión

    btnIniciarSesion.cuadro.w = ancho_botones;
    btnIniciarSesion.cuadro.h = alto_botones;
    btnIniciarSesion.padding = padding;
    btnIniciarSesion.bg_color = COLOR(BG_COLOR_1, OPACO);
    btnIniciarSesion.bd_color = COLOR(BD_COLOR_1, OPACO);
    btnIniciarSesion.fg_color = COLOR(FG_COLOR_1, OPACO);
    btnIniciarSesion.text_align = TEXT_ALIGN_CENTER;
    SDL_asprintf(&btnIniciarSesion.text, TEXTO_INICIAR_SESION);

    // Segundo botón: Crear cuenta

    btnCrearCuenta.cuadro.w = ancho_botones;
    btnCrearCuenta.cuadro.h = alto_botones;
    btnCrearCuenta.padding = padding;
    btnCrearCuenta.bg_color = COLOR(BG_COLOR_2, OPACO);
    btnCrearCuenta.bd_color = COLOR(BD_COLOR_2, OPACO);
    btnCrearCuenta.fg_color = COLOR(FG_COLOR_2, OPACO);
    btnCrearCuenta.text_align = TEXT_ALIGN_CENTER;
    SDL_asprintf(&btnCrearCuenta.text, TEXTO_CREAR_CUENTA);

    // Tercer botón: Continuar como invitado

    btnInvitado.cuadro.w = ancho_botones;
    btnInvitado.cuadro.h = alto_botones;
    btnInvitado.padding = padding;
    btnInvitado.bg_color = COLOR(BG_COLOR_3, OPACO);
    btnInvitado.bd_color = COLOR(BD_COLOR_3, OPACO);
    btnInvitado.fg_color = COLOR(FG_COLOR_3, OPACO);
    btnInvitado.text_align = TEXT_ALIGN_CENTER;
    SDL_asprintf(&btnInvitado.text, TEXTO_INVITADO);

    VibeCast_SetRenderScale(2.0f, 2.0f);

    initialized = true; // Se ha inicializado el login
}

/* Función para actualizar los objetos de la ventana login */
void update()
{
    /* Por ejemplo, puedo decir que el botón vaya cambiando su tamaño según el tamaño de la ventana */
    /* Cambiar tamaño botones según tamaño ventana */

    int w, h; // Para el tamaño de la ventana
    VibeCast_GetRenderSize(&w, &h);

    w /= 2.0f;
    h /= 2.0f;

    int center_x = w / 2;
    int center_y = h / 2;
    int sep = 20;

    btnIniciarSesion.cuadro.x = center_x - btnIniciarSesion.cuadro.w / 2;
    btnCrearCuenta.cuadro.x = center_x - btnCrearCuenta.cuadro.w / 2;
    btnInvitado.cuadro.x = center_x - btnInvitado.cuadro.w / 2;

    btnCrearCuenta.cuadro.y = center_y - btnCrearCuenta.cuadro.h / 2;
    btnIniciarSesion.cuadro.y = btnCrearCuenta.cuadro.y - btnIniciarSesion.cuadro.h - sep;
    btnInvitado.cuadro.y = center_y + (btnCrearCuenta.cuadro.h / 2) + sep;
}

/* En esta función se renderizan los elementos */
void render()
{
    /* Dibujar elementos visuales */
    /* Mostrar/renderizar los objetos visuales */

    VibeCast_ClearScreen(COLOR(BG_COLOR, OPACO)); // Limpiar la pantalla con el color dado

    VibeCast_Render(&btnIniciarSesion);
    VibeCast_Render(&btnCrearCuenta);
    VibeCast_Render(&btnInvitado);

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
    // return SDL_APP_SUCCESS;
}
