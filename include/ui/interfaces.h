#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H 1

#include <VibeCastConfig.h>
#include <stdbool.h>
#include <db/datos.h>
#include <ui/conexiones.h>

// Función para liberar la memoria de la expresión regular utilizada para validar emails.
void free_re();

// Usuario activo
extern Usuario *usuario;

// Cola de reproducción
extern Cola cola_repr;

/* Funciones principales */

#define custom_interface(name, ...) bool func(name, void *arg, __VA_ARGS__, char **msg)
#define interfaz(name) custom_interface(name, int argc, const char *argv[])

#define required(expr, msg)    \
    if (!(expr))               \
    {                          \
        VibeCast_SendError(    \
            id,                \
            HTTP_UNAUTHORIZED, \
            msg,               \
            STATE_FAILURE);    \
        return;                \
    }

#define login_required() required(usuario, "Inicio de sesión requerido.")

// Macro para facilitar enviar mensajes a las conexiones (message_handler)
#define send_message(...) \
    if (msg)              \
    *msg = asprintf(__VA_ARGS__)

/* ==== Funciones públicas ==== */

new_operfn(getSongJSON);

new_operfn(rehacerHistorial);

#endif // VIBECAST_INTERFACES_H