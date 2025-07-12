#include <ui/interfaces.h>

interfaz(activarPremium)
{
    time_t base = time(NULL);

    if (base == -1)
    {
        send_message("Error al obtener la fecha actual\n");
        return false;
    }

    if (usuario->plan != PLAN_PREMIUM)
    {
        usuario->plan = PLAN_PREMIUM;
        send_message("¡Plan Premium activado!");
    }
    else
    {
        base = usuario->caducidadPremium;
        send_message("¡Plan Premium renovado!");
    }

    usuario->caducidadPremium = base + 30 * 24 * 60 * 60; // Tiempo en segundos
    // La fecha es imprime como dia de la semana, mes, dia del mes, hora, minuto, segundo y año
    send_message("Caduca el: %s", ctime(&usuario->caducidadPremium));

    return true;
}

message_handler(activarPremium)
{

    init_data_json(); // Inicializa el JSON de entrada (aunque no hay argumentos)

    char **msg = arg;
    bool success = VibeCast_activarPremium(usuario, 0, NULL, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        success ? "Plan activado o renovado" : "Fallo al activar Premium",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}
//--------------------------------------------------------------------------------//

interfaz(desactivarPremium)
{
    if (usuario->plan == PLAN_PREMIUM)
    {
        usuario->plan = PLAN_FREEMIUM; // Cambiamos el plan del usuario a freemium
        usuario->caducidadPremium = 0; // Reseteamos la fecha de caducidad del plan premium
        send_message("¡Plan Premium desactivado!");
    }
    else
    {
        send_message("No estás en el plan Premium, no puedes desactivarlo.");
    }

    return true;
}

message_handler(desactivarPremium)
{
    init_data_json();

    char **msg = arg;
    bool success = VibeCast_desactivarPremium(usuario, 0, NULL, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        "Plan Freemium activado",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

//--------------------------------------------------------------------------------//

interfaz(esUsuarioPremium)
{

    time_t ahora = time(NULL);
    if (ahora == -1)
    {
        send_message("Error al obtener la fecha actual\n");
        return false;
    }

    if (usuario->caducidadPremium <= ahora)
    {
        send_message("Tu plan Premium ha caducado. Por favor, renueva tu suscripción\n");
        VibeCast_desactivarPremium(usuario, 0, NULL, NULL); // Desactiva el plan si ha caducado
        return false;                                       // Retorna false si el usuario no es premium
    }

    return usuario->plan == PLAN_PREMIUM; // Retorna true si el usuario es premium
}

message_handler(esUsuarioPremium)
{
    VibeCast_SendBool(
        id,
        HTTP_OK,
        usuario->plan == PLAN_PREMIUM,
        "",
        STATE_SUCCESS);
}

//--------------------------------------------------------------------------------//
interfaz(renovarPremium)
{
    VibeCast_activarPremium(usuario, 0, NULL, NULL); // Llama a la función para activar el plan premium
    send_message("Tu plan Premium ha sido renovado exitosamente\n");

    return true;
}

message_handler(renovarPremium)
{
    init_data_json();

    char **msg = arg;
    bool success = VibeCast_renovarPremium(usuario, 0, NULL, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        success ? "Renovación exitosa" : "Error al renovar",
        STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}
