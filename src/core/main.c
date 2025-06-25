#include <VibeCastConfig.h>

#define MAIN_USE_CALLBACKS
#define WINMAIN
#include <core/main.h>

#include <ui/gui.h>   // Para la interfaz gráfica
#include <db/datos.h> // Para cargar los datos
#include <stdio.h>    // Para imprimir mensajes en consola.

/* ======== Funciones principales de la aplicación ======== */

AppResult AppInit(void **appstate, int argc, char *argv[])
{
    // Inicializar la interfaz gráfica
    if (!func(InitGUI, *appstate))
    {
        puts("Error al inicializar la interfaz gráfica.");
        return APP_FAILURE;
    }

    // Cargar datos
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
    return func(RunGUI, appstate) ? APP_SUCCESS : APP_FAILURE;
}

void AppQuit(void *appstate, AppResult appresult)
{
    func(DestroyGUI, appstate); // Liberar la memoria de la interfaz gráfica
    func(FreeData);   // Para liberar la memoria de las estructuras de datos
    free(appstate);   // Liberar la memoria de appstate

    puts("Ejecución finalizada");
}
