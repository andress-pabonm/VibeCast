#ifndef VIBECAST_GUI_H
#define VIBECAST_GUI_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>

/* Funciones principales */

bool VibeCast_InitGUI();    // Inicializar la interfaz gráfica
bool VibeCast_RunGUI();     // Para ejecutar la interfaz gráfica
bool VibeCast_DestroyGUI(); // Liberar la memoria de la interfaz gráfica

#endif // VIBECAST_GUI_H