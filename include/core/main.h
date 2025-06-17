#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>

/**
 * Tipo personalizado para representar las constantes para los resultados de la aplicación.
 */
typedef enum
{
    // Código para la finalización normal.
    APP_SUCCESS = EXIT_SUCCESS,

    // Código para la finalización con error.
    APP_FAILURE = EXIT_FAILURE,

    // Código para continuar con la ejecución de la aplicación.
    APP_CONTINUE = 2
} AppResult;

/**
 * Función para inicializar la aplicación. Esta función se llama una vez al ejecutar la aplicación.
 *
 * @param appstate: Puntero a una variable donde se guardará el estado de la aplicación, el cual se provee a AppIter y AppQuit. Por defecto es NULL.
 * @param argc: Cantidad de argumentos al ejecutar la aplicación.
 * @param argv: Lista de argumentos al ejecutar la aplicación.
 *
 * @return Constante AppResult.
 */
AppResult AppInit(void **appstate, int argc, char *argv[]);

/**
 * Función para el bucle principal de ejecución de la aplicación.
 *
 * @param appstate: Puntero al valor inicializado en AppInit.
 *
 * @return Constante AppResult.
 */
AppResult AppIterate(void *appstate);

/**
 * Función para finalizar la aplicación. Se ejecuta mientras se retorne APP_CONTINUE.
 *
 * @param appstate: Puntero al valor inicializado en AppInit.
 * @param appresult: Código con el que finaliza la aplicación.
 */
void AppQuit(void *appstate, AppResult appresult);

/**
 * Función principal de cualquier programa de C.
 */
int main(int argc, char *argv[]);

#ifdef MAIN_USE_CALLBACKS

int main(int argc, char *argv[])
{
    void *appstate = NULL;
    AppResult appresult;

    appresult = AppInit(&appstate, argc, argv);

    while (appresult == APP_CONTINUE)
    {
        appresult = AppIterate(appstate);
    }

    AppQuit(appstate, appresult);

    return appresult;
}

#endif // MAIN_USE_CALLBACKS

#if defined(WINMAIN) && defined(_WIN32)

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    // Llama a tu función principal "real"
    return main(__argc, __argv); // solo en MSVC
}

#endif // WINMAIN && _WIN32

#endif // MAIN_H
