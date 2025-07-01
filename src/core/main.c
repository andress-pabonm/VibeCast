#include <VibeCastConfig.h>

#define VIBECAST_WINMAIN 1
#define VIBECAST_MAIN_CALLBACKS 1
#include <core/main.h>

#include <ui/gui.h>   // Para la interfaz gráfica
#include <db/datos.h> // Para cargar los datos
#include <utils/utils.h>

/* ======== Funciones principales de la aplicación ======== */

AppResult VibeCast_AppInit(void **appstate, int argc, const char *argv[])
{
    // Inicializar la interfaz gráfica
    if (!VibeCast_InitGUI())
    {
        puts("Error al inicializar la interfaz gráfica.");
        return VIBECAST_APP_FAILURE;
    }

    puts("Interfaz gráfica inicializada");

    char *errmsg = NULL;

    // Cargar datos
    if (!VibeCast_LoadData(&errmsg))
    {
        puts(errmsg);
        freem(errmsg);
        return VIBECAST_APP_FAILURE;
    }

    puts("Datos cargados");

    puts("Aplicación inicializada correctamente.");

    return VIBECAST_APP_CONTINUE; // Continuar con la ejecución
}

AppResult VibeCast_AppIter(void *appstate)
{
    return VibeCast_RunGUI()
               ? VIBECAST_APP_SUCCESS
               : VIBECAST_APP_FAILURE;
}

AppResult VibeCast_AppQuit(void *appstate, AppResult appresult)
{
    VibeCast_DestroyGUI(appstate); // Liberar la memoria de la interfaz gráfica
    VibeCast_FreeData();           // Para liberar la memoria de las estructuras de datos
    freem(appstate);               // Liberar la memoria de appstate

    printf("appresult: %d\n", appresult);

    puts("Ejecución finalizada");

    return appresult;
}
