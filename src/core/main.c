#include <VibeCastConfig.h>

#define VIBECAST_WINMAIN 1
#define VIBECAST_MAIN_CALLBACKS 1
#include <core/main.h>

#include <ui/gui.h>   // Para la interfaz gráfica
#include <db/datos.h> // Para cargar los datos
#include <stdio.h>    // Para imprimir mensajes en consola.

/* ======== Funciones principales de la aplicación ======== */

AppResult VibeCast_AppInit(void **appstate, int argc, char *argv[])
{
    // Inicializar la interfaz gráfica
    if (!func(InitGUI, *appstate))
    {
        puts("Error al inicializar la interfaz gráfica.");
        return VIBECAST_APP_FAILURE;
    }

    // Cargar datos
    if (!func(LoadData))
    {
        puts("Error al cargar los datos.");
        return VIBECAST_APP_FAILURE;
    }

    puts("Aplicación inicializada correctamente.");

    return VIBECAST_APP_FAILURE; // Continuar con la ejecución
}

AppResult AppIterate(void *appstate)
{
    return func(RunGUI, appstate)
               ? VIBECAST_APP_SUCCESS
               : VIBECAST_APP_FAILURE;
}

AppResult VibeCast_AppQuit(void *appstate, AppResult appresult)
{
    func(DestroyGUI, appstate); // Liberar la memoria de la interfaz gráfica
    func(FreeData);             // Para liberar la memoria de las estructuras de datos
    free(appstate);             // Liberar la memoria de appstate

    puts("Ejecución finalizada");

    return appresult;
}
