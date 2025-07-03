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

bool VibeCast_InitGUI()
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

    VibeCast_InitBindings();

    // Ir a la primera interfaz
    webview_navigate(w, INTERFAZ("Login/index.html"));

    return true;
}

bool VibeCast_RunGUI()
{
    return webview_run(w) == WEBVIEW_ERROR_OK;
}

bool VibeCast_DestroyGUI()
{
    return webview_destroy(w) == WEBVIEW_ERROR_OK;
}

// Para enlazar una función a la interfaz gráfica
bool VibeCast_BindFn(const char *name, message_handler_t msgh, void *arg)
{
    return webview_bind(w, name, msgh, arg) == WEBVIEW_ERROR_OK;
}

/* ================================================================ */

// Función base: acepta json_object* para data
bool VibeCast_SendObj(const char *id, int status_code, const char *type, json_object *data, const char *msg, const char *status)
{
    if (!type)
        type = "text";
    if (!status)
        status = "ok";

    json_object *root = json_object_new_object();

    json_object_object_add(root, "type", json_object_new_string(type));
    json_object_object_add(root, "data", data ? json_object_get(data) : json_object_new_null());
    json_object_object_add(root, "status", json_object_new_string(status));
    json_object_object_add(root, "code", json_object_new_int(status_code));
    json_object_object_add(root, "message", msg ? json_object_new_string(msg) : json_object_new_null());

    const char *json_str = json_object_to_json_string(root);
    int logic_error = (status_code >= 400);
    bool ok = webview_return(w, id, logic_error, json_str) == WEBVIEW_ERROR_OK;

    json_object_put(root); // libera el objeto raíz (los hijos quedan liberados si se hizo get)
    return ok;
}

// Envía un valor booleano (true/false)
bool VibeCast_SendBool(const char *id, int status_code, bool value, const char *msg, const char *status)
{
    json_object *val = json_object_new_boolean(value);
    bool ok = VibeCast_SendObj(id, status_code, "boolean", val, msg, status);
    json_object_put(val);
    return ok;
}

// Envía un número entero
bool VibeCast_SendInt(const char *id, int status_code, int value, const char *msg, const char *status)
{
    json_object *val = json_object_new_int(value);
    bool ok = VibeCast_SendObj(id, status_code, "int", val, msg, status);
    json_object_put(val);
    return ok;
}

// Envía un número decimal (double)
bool VibeCast_SendFloat(const char *id, int status_code, double value, const char *msg, const char *status)
{
    json_object *val = json_object_new_double(value);
    bool ok = VibeCast_SendObj(id, status_code, "float", val, msg, status);
    json_object_put(val);
    return ok;
}

// Envía una cadena de texto (más directo que SendMsg)
bool VibeCast_SendText(const char *id, int status_code, const char *text, const char *msg, const char *status)
{
    json_object *val = text ? json_object_new_string(text) : json_object_new_null();
    bool ok = VibeCast_SendObj(id, status_code, "text", val, msg, status);
    json_object_put(val);
    return ok;
}

// Envía un array JSON
bool VibeCast_SendArray(const char *id, int status_code, json_object *array, const char *msg, const char *status)
{
    if (!array || !json_object_is_type(array, json_type_array))
        array = json_object_new_array(); // fallback

    return VibeCast_SendObj(id, status_code, "array", array, msg, status);
}

// Envía un objeto JSON
bool VibeCast_SendJSON(const char *id, int status_code, json_object *object, const char *msg, const char *status)
{
    if (!object || !json_object_is_type(object, json_type_object))
        object = json_object_new_object(); // fallback

    return VibeCast_SendObj(id, status_code, "json", object, msg, status);
}

// Envía un valor nulo explícito
bool VibeCast_SendNull(const char *id, int status_code, const char *msg, const char *status)
{
    json_object *val = json_object_new_null();
    bool ok = VibeCast_SendObj(id, status_code, "null", val, msg, status);
    json_object_put(val);
    return ok;
}
