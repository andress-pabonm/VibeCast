#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>
#include <db/datos.h>
#include <ui/conexiones.h>

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

// Macro para facilitar enviar mensajes a las conexiones (message_handler)
#define send_message(...) \
    if (msg)              \
    *msg = asprintf(__VA_ARGS__)

Lista recomendarCanciones();

json_object *cancion_to_json(Cancion *cancion);
new_operfn(getCanciones);

interfaz(CrearCancion);

#endif // VIBECAST_INTERFACES_H