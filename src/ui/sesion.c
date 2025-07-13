#include <ui/interfaces.h>

/* ================================================================ */
// DECLARACIÓN DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(SesionActiva);
static interfaz(IniciarSesion);
static interfaz(CerrarSesion);

/* ================================================================ */
// VARIABLE GLOBAL DE SESIÓN ACTIVA
/* ================================================================ */

Usuario *usuario = NULL;

/* ================================================================ */
// BLOQUE: sesion_activa — Verificar si hay sesión activa
/* ================================================================ */

message_handler(sesion_activa)
{
    char **msg = arg;

    bool success = VibeCast_SesionActiva(NULL, 0, NULL, msg);
    VibeCast_SendBool(id, HTTP_OK, success, *msg, STATE_SUCCESS);

    freem(*msg);
    *msg = NULL;
}

static interfaz(SesionActiva)
{
    if (!usuario)
    {
        send_message("No ha iniciado sesión.");
        return false;
    }

    send_message("Ha iniciado sesión.");
    return true;
}

/* ================================================================ */
// BLOQUE: iniciar_sesion — Manejo del inicio de sesión
/* ================================================================ */

message_handler(iniciar_sesion)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0));
    const char *password = get_string(get_array_idx(data, 1));

    int argc = 2;
    const char *argv[] = {username, password};
    char **msg = arg;

    bool success = VibeCast_IniciarSesion(NULL, argc, argv, msg);
    VibeCast_SendBool(id, HTTP_OK, success, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(IniciarSesion)
{
    const char *username = argv[0];
    const char *password = argv[1];

    usuario = searchValueInABB(usuarios, username, cmpUsuarioConUsername);
    if (!usuario)
    {
        send_message("Usuario no registrado");
        return false;
    }

    if (strcmp(usuario->password, password))
    {
        usuario = NULL;
        send_message("Contraseña incorrecta");
        return false;
    }

    send_message("Inicio de sesión exitoso");
    return true;
}

/* ================================================================ */
// BLOQUE: cerrar_sesion — Cerrar la sesión activa
/* ================================================================ */

message_handler(cerrar_sesion)
{
    char **msg = arg;

    bool success = VibeCast_CerrarSesion(NULL, 0, NULL, msg);
    VibeCast_SendBool(id, HTTP_OK, success, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
}

static interfaz(CerrarSesion)
{
    usuario = NULL;
    send_message("Sesión cerrada.");
    return true;
}
