#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H 1

#include <VibeCastConfig.h>
#include <db/estructuras.h>

// Función para liberar la memoria de la expresión regular utilizada para validar emails.
void free_re();

/**
 * Variables globales
 * Estas estructuras estarán disponibles
 * para todos los archivos que incluyan
 * interfaces.h
 * #include <ui/interfaces.h>
 */

extern Usuario *usuario;
extern Cola cola_repr;

/* Funciones principales */

#define interfaz(name) func(name, int argc, const char *argv[], char **msg)

interfaz(IsLoggedIn);
interfaz(IniciarSesion);
interfaz(CerrarSesion);
interfaz(CrearCuenta);

#endif // VIBECAST_INTERFACES_H