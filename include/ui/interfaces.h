#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>
#include <db/datos.h>

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

#define interfaz(name) bool func(name, void *arg, int argc, const char *argv[], char **msg)

interfaz(IsLoggedIn);    // Para verificar si se ha iniciado sesión
interfaz(IniciarSesion); // Para iniciar sesión
interfaz(CerrarSesion);  // Para cerrar sesión
interfaz(CrearCuenta);   // Para crear una nueva cuenta

// Función para avanzar en la cola de reproducción
interfaz(SiguienteCancion);

#endif // VIBECAST_INTERFACES_H