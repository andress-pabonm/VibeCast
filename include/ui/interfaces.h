#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

#include <utils/utils.h>

// Función para liberar la memoria de la expresión regular utilizada para validar emails.
void free_re();

/* Funciones principales */

#define interfaz(name) func(name, int argc, const char *argv[], char **msg)

interfaz(IsLoggedIn);
interfaz(IniciarSesion);
interfaz(CerrarSesion);
interfaz(CrearCuenta);

#endif // VIBECAST_INTERFACES_H