#include <VibeCastConfig.h>

#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

#include <db/dbmgr.h>
#include <utils/utils.h>
#include <ui/interfaces.h>

#include <webview/webview.h> // Para la interfaz gráfica
#include <json.h>            // Para recibir las peticiones desde JS
#include <stdio.h>           // Mientras se prueba la aplicación.
#include <string.h>

// Definir tamaños mínimos y máximos
// #define MIN_WIDTH 320
// #define MIN_HEIGHT 480
#define INIT_WIDTH 640
#define INIT_HEIGHT 720

#define JSON(...) stringify({__VA_ARGS__})

typedef void (*message_handler_t)(const char *id, const char *req, void *arg);
#define new_message_handler(name) void name(const char *id, const char *req, void *arg)

new_message_handler(handle_message);

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

typedef struct
{
    webview_t w;
} AppState;

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

    // Icono de la ventana
    HICON hIcon = (HICON)LoadImage(NULL, path_to("assets/icon/favicon.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    // Enlazar funciones para controlar la interfaz gráfica
    webview_bind(w, "enviarMensaje", handle_message, *appstate);

    // Idealmente habría que crear un controlador para cada interfaz
    // De ese modo se facilitaría distinguir en qué interfaz estaría el usuario

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
    if (nuevo_registro(
            "Usuarios",
            "email, password, username, nickname, pais, plan, nombre_artista",
            stringify(
                "rociovc@gmail.com",
                "abduzcan",
                "rocio",
                "rocioxdxdlol",
                "Afganistan",
                0, // esta pobre XD
                "rociovcoficial1645"),
            NULL))
    {
        puts("Registro exitoso.");
    }
    else
    {
        puts("Registro fallido.");
    }

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
    free(appstate);

    puts("Ejecución finalizada");
}

new_select_handler(comprobarUsuario)
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s = %s\n", fields[i], argv[i]);
    }

    return 0;
}

new_message_handler(handle_message)
{
    printf("req: %s\n", req);

    // TODO 1: Obtener el tipo de petición que se está haciendo

    // Convertir un string a un objeto JSON
    struct json_object *array = json_tokener_parse(req);

    // Extrae el primer elemento de la lista
    struct json_object *json_str_obj = json_object_array_get_idx(array, 0);

    const char *json_inner_str = json_object_get_string(json_str_obj);
    struct json_object *parsed_json = json_tokener_parse(json_inner_str);

    struct json_object *type, *data;

    json_object_object_get_ex(parsed_json, "type", &type);
    json_object_object_get_ex(parsed_json, "data", &data);

    const char *type_str = json_object_get_string(type);
    printf("type: %s\n", type_str);

    // TODO 2: Según el tipo de petición, invocar a las diferentes funciones
    if (!strcmp(type_str, "login"))
    {
        puts("Estás intentando iniciar sesión.");

        struct json_object *email, *password;

        json_object_object_get_ex(data, "email", &email);
        json_object_object_get_ex(data, "password", &password);

        const char *email_str = json_object_get_string(email);
        const char *password_str = json_object_get_string(password);

        printf("%s %s\n", email_str, password_str);

        char condition[256];
        snprintf(condition, sizeof(condition),
                 stringify(email = "%s"), email_str);

        obtener_registros(
            "*",
            "Usuarios",
            condition,
            comprobarUsuario,
            NULL,
            NULL);
    }

    // TODO 3: Enviar un mensaje de respuesta a JS

    webview_return(cast(AppState *, arg)->w, id, 0, JSON("status" : "OK"));
}
