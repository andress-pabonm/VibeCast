#include <ui/gui.h>
#include <webview/webview.h>
#include <windows.h>
#include <ui/conexiones.h>
#include <utils/utils.h>

/**
 * Macro para facilitar la llamada a los archivos .html
 * Utilizas la ruta relativa al directorio ui donde están las .html
 * Uso: INTERFAZ("carpeta/archivo.html")
 */
#define INTERFAZ(nombre) "file://" path_to("ui/" nombre)

/* Para el tamañño de la ventana */

#define MIN_WIDTH 480
#define MIN_HEIGHT 640
#define INIT_WIDTH MIN_WIDTH
#define INIT_HEIGHT MIN_HEIGHT

// Guardar el procedimiento de ventana original
static WNDPROC original_wndproc = NULL;

// Nuevo WndProc que fuerza un tamaño mínimo
static LRESULT CALLBACK MinSizeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

// Para centrar la ventana en la pantalla
static void centrarVentana(HWND hwnd)
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

#define setTitle(hwnd, title) SetWindowText(hwnd, title)
#define setPosAndSize(hwnd, x, y, w, h) SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_SHOWWINDOW)
#define setMaximized(hwnd) ShowWindow(hwnd, SW_SHOWMAXIMIZED)
#define setIcon(hwnd, url)                                                                         \
    HICON hIcon = (HICON)LoadImage(NULL, url, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE); \
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon)

static webview_t w = NULL;

bool func(InitGUI, void *appstate)
{
    // Crear la ventana
    w = webview_create(1, NULL);       // Crea la ventana con opción para Debug (Inspecionar)
    HWND hwnd = webview_get_window(w); // Obtiene el controlador nativo de la ventana

    // Configuración inicial de la ventana

    // Título de la ventana
    setTitle(hwnd, "VibeCast");

    // Posición, tamaño y características similares
    setPosAndSize(hwnd, 0, 0, INIT_WIDTH, INIT_HEIGHT);
    centrarVentana(hwnd);

    // Reemplazar el WndProc por uno personalizado
    // Se utiliza para manejar eventos en la ventana
    // En este caso para manejar el tamaño mínimo de la ventana
    original_wndproc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
    SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)MinSizeWndProc);

    // Maximizar la ventana
    setMaximized(hwnd);

    // Ícono de la ventana
    setIcon(hwnd, path_to("assets/icon/favicon.ico"));

    func(InitBindings);

    // Ir a la primera interfaz
    webview_navigate(w, INTERFAZ("Login/index.html"));

    return true;
}

bool func(RunGUI, void *appstate)
{
    return webview_run(w) == WEBVIEW_ERROR_OK;
}

bool func(DestroyGUI, void *appstate)
{
    return webview_destroy(w) == WEBVIEW_ERROR_OK;
}

// Para enlazar una función a la interfaz gráfica
bool func(BindFn, const char *name, message_handler_t msgh, void *arg)
{
    return webview_bind(w, name, msgh, arg) == WEBVIEW_ERROR_OK;
}

/* Para enviar un mensaje a la interfaz gráfica */
bool func(SendMsg, const char *id, int status, const char *msg)
{
    if (!msg)
        msg = "";

    char *json_message =
        asprintf(
            stringify({"message" : "%s"}),
            msg);
    bool ok = webview_return(w, id, status, json_message) == WEBVIEW_ERROR_OK;
    free(json_message);
    return ok;
}
