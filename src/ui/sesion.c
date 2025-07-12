#include <ui/interfaces.h>

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

message_handler(sesion_activa)
{
    VibeCast_SendBool(id, HTTP_OK, usuario != NULL, NULL, STATE_SUCCESS);
}

message_handler(iniciar_sesion)
{
    init_data_json();
    const char *username = get_string(get_array_idx(data, 0));
    const char *password = get_string(get_array_idx(data, 1));

    char **msg = arg;
    bool ok = VibeCast_IniciarSesion(NULL, 2, cast(const char *[], username, password), msg);
    VibeCast_SendBool(id, ok ? HTTP_OK : HTTP_BAD_REQUEST, ok, *msg, ok ? STATE_SUCCESS : STATE_FAILURE);
    freem(*msg);
    *msg = NULL;
}

message_handler(cerrar_sesion)
{
    usuario = NULL;
    VibeCast_SendBool(id, HTTP_OK, true, "Sesión cerrada", STATE_SUCCESS);
}
