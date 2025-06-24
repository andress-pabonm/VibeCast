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

#define MIN_WIDTH 320
#define MIN_HEIGHT 480
// Anchura inicial de la ventana
#define INIT_WIDTH 640
// Altura inicial de la ventana
#define INIT_HEIGHT 720

/* ==== Utilidades ==== */

// Para centrar la ventana en la pantalla
void centrar_ventana(HWND hwnd);

// Guardar el procedimiento de ventana original
static WNDPROC original_wndproc = NULL;

// Nuevo WndProc que fuerza un tamaño mínimo
LRESULT CALLBACK MinSizeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_GETMINMAXINFO)
    {
        MINMAXINFO *minmax = (MINMAXINFO *)lParam;

        // minmax->ptMinTrackSize.x = 480; // Ancho mínimo
        // minmax->ptMinTrackSize.y = 360; // Alto mínimo

        minmax->ptMinTrackSize.x = MIN_WIDTH;
        minmax->ptMinTrackSize.y = MIN_HEIGHT;

        return 0;
    }

    // Llamar al WndProc original para otros mensajes
    return CallWindowProc(original_wndproc, hwnd, msg, wParam, lParam);
}

/* ======== Funciones principales de la aplicación ======== */

AppResult AppInit(void **appstate, int argc, char *argv[])
{
    /* ======== Inicializar el estado de la aplicación ======== */

    AppState *state = malloc(sizeof(AppState));
    if (!state) // Si no se pudo reservar memoria para appstate
        return APP_FAILURE;
    *appstate = state;

    puts("Estado de la aplicación inicializado.");

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

    // Reemplazar el WndProc por uno personalizado
    original_wndproc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MinSizeWndProc);

    // Maximizar la ventana
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);

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
    webview_navigate(w, INTERFAZ("Login/index.html"));

    puts("Interfaz gráfica inicializada");

    /* ======== Inicializar la base de datos ======== */

    char *errmsg = NULL;

    // if (!func(InitDB, ":memory:", "db_script.txt", NULL)) // Para hacer pruebas
    if (!func(InitDB, "data.db", "db_script.txt", &errmsg))
    {
        puts("Error al inicializar la base de datos.");
        puts(errmsg);
        free_errmsg(errmsg);
        return APP_FAILURE;
    }

    puts("Base de datos inicializada.");

    /* ======== Cargar los datos e inicializar las estructuras ======== */

    if (!func(LoadData))
    {
        puts("Error al cargar los datos.");
        return APP_FAILURE;
    }

    puts("Datos cargados.");

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

    // TODO: Implementar la liberación de memoria de las estructuras
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
