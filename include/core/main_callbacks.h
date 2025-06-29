#ifndef VIBECAST_MAIN_CALLBACKS_H
#define VIBECAST_MAIN_CALLBACKS_H 1

#include <stdlib.h>

/**
 * Códigos devueltos por algunas funciones
 *
 * Códigos disponibles:
 *
 * VIBECAST_APP_SUCCESS: Código de finalización correcta.
 * VIBECAST_APP_FAILURE: Código de finalización fallida.
 * VIBECAST_APP_CONTINUE: Código para continuar con la ejecución.
 */
typedef enum
{
    // Código de finalización correcta.
    VIBECAST_APP_SUCCESS = EXIT_SUCCESS,

    // Código de finalización fallida.
    VIBECAST_APP_FAILURE = EXIT_FAILURE,

    // Código para continuar con la ejecución.
    VIBECAST_APP_CONTINUE
} AppResult;

AppResult VibeCast_AppInit(void **appstate, int argc, const char *argv[]);
AppResult VibeCast_AppIter(void *appstate);
AppResult VibeCast_AppQuit(void *appstate, AppResult appresult);

#ifdef VIBECAST_MAIN_CALLBACKS

int main(int argc, const char *argv[])
{
    void *appstate = NULL;
    AppResult appresult = VibeCast_AppInit(&appstate, argc, argv);
    while (appresult == VIBECAST_APP_CONTINUE)
        appresult = VibeCast_AppIter(appstate);
    return VibeCast_AppQuit(appstate, appresult);
}

#endif // VIBECAST_MAIN_CALLBACKS

#endif // VIBECAST_MAIN_CALLBACKS_H