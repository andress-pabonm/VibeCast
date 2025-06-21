#include <VibeCastConfig.h>

#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

// Incluir las funciones necesarias para inicializar la app
#include <ui/interfaces.h> // Para las funciones tipo message_handler_t y el webview.h
#include <db/dbmgr.h>      // Para InitDB y CloseDB
#include <utils/utils.h>   // Para malloc_cpy()

#include <stdio.h> // Para imprimir mensajes en consola.

/* ==== Constantes para las dimensiones de la ventana ==== */

// #define MIN_WIDTH 320
// #define MIN_HEIGHT 480
// Anchura inicial de la ventana
#define INIT_WIDTH 640
// Altura inicial de la ventana
#define INIT_HEIGHT 720

/* ==== Utilidades ==== */

// Para centrar la ventana en la pantalla
void centrar_ventana(HWND hwnd);

/* ======== Funciones principales de la aplicación ======== */

AppResult AppInit(void **appstate, int argc, char *argv[])
{
    /* ======== Inicializar el estado de la aplicación ======== */

    AppState *state = malloc(sizeof(AppState));
    if (!state) // Si no se pudo reservar memoria para appstate
        return APP_FAILURE;
    *appstate = state;

    /* ======== Inicializar la interfaz gráfica ======== */

    // Crear la ventana
    // state->w = webview_create(0, NULL); // Descomentar para quitar Debug Tools
    state->w = webview_create(1, NULL); // Crea la ventana con opción para Debug (Inspecionar)
    webview_t w = state->w;
    HWND hwnd = webview_get_window(w); // Obtiene el controlador nativo de la ventana

    // Configuración inicial de la ventana

    // Título de la ventana
    SetWindowText(hwnd, "VibeCast");

    // Posición, tamaño y características similares
    SetWindowPos(hwnd, NULL, 0, 0, INIT_WIDTH, INIT_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);
    centrar_ventana(hwnd);

    // Ícono de la ventana
    HICON hIcon = (HICON)LoadImage(NULL, path_to("assets/icon/favicon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    // Enlazar funciones para controlar la interfaz gráfica
    bind_fn(w, test, *appstate);

    // Idealmente habría que crear un controlador para cada interfaz
    // De ese modo se facilitaría distinguir en qué interfaz estaría el usuario

    bind_fn(w, is_logged_in, *appstate);
    bind_fn(w, iniciar_sesion, *appstate);
    bind_fn(w, cerrar_sesion, *appstate);
    bind_fn(w, crear_cuenta, *appstate);

    // Ir a la primera interfaz
    //webview_navigate(w, INTERFAZ("Login/index.html")); necesito ya ir viendo el modelo del menu y perfil

    webview_navigate(w, INTERFAZ("Menu/menu.html"));

    /* ======== Inicializar la base de datos ======== */

    if (!func(InitDB, ":memory:", NULL))
    {
        puts("Error al inicializar la base de datos.");
        return APP_FAILURE;
    }

    /* ======== Insertar datos de prueba ======== */

    // Aquí se carga un registro de prueba
    // if (nuevo_registro(
    //         "Usuarios",
    //         "email, password, username, nickname, pais, plan, nombre_artista",
    //         stringify(
    //             "rociovc@gmail.com",
    //             "abduzcan",
    //             "rocio",
    //             "rocioxdxdlol",
    //             "Afganistan",
    //             0, // esta pobre XD
    //             "rociovcoficial1645"),
    //         NULL))
    // {
    //     puts("Registro exitoso.");
    // }
    // else
    // {
    //     puts("Registro fallido.");
    // }

    /* ======== Cargar los datos e inicializar las estructuras ======== */

    if (!func(LoadData))
    {
        puts("Error al cargar los datos.");
        return APP_FAILURE;
    }

    puts("Aplicación inicializada correctamente.");

    return APP_CONTINUE; // Continuar con la ejecución
}

AppResult AppIterate(void *appstate)
{
    switch (webview_run(cast(AppState *, appstate)->w)) // Ejecutar el bucle principal de la ventana
    {
    case WEBVIEW_ERROR_OK: // No hubo errores
        return APP_SUCCESS;

    default: // Hubo un error
        return APP_FAILURE;
    }
}

void AppQuit(void *appstate, AppResult appresult)
{
    webview_destroy(cast(AppState *, appstate)->w); // Destruir la ventana
    // func(FreeData);                                 // Para liberar la memoria de las estructuras de datos
    func(CloseDB); // Cerrar la base de datos

    free(appstate);

    puts("Ejecución finalizada");
}

/* ======== Definición de funciones adicionales ======== */

void centrar_ventana(HWND hwnd)
{
    RECT rect;
    int anchoVentana, altoVentana;
    int anchoPantalla = GetSystemMetrics(SM_CXSCREEN);
    int altoPantalla = GetSystemMetrics(SM_CYSCREEN);

    GetWindowRect(hwnd, &rect);
    anchoVentana = rect.right - rect.left;
    altoVentana = rect.bottom - rect.top;

    // Calcular posición centrada
    int x = (anchoPantalla - anchoVentana) / 2;
    int y = (altoPantalla - altoVentana) / 2;

    // Mover la ventana
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
}
