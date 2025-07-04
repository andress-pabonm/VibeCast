#include <ui/interfaces.h>
#include <time.h>

/* ================================ Interfaces ================================ */



static message_handler(crear_artista)
{
    // A implementar
}

/* ================================================================ */

static json_object *exportar_usuario_json(const char *name, const char *username, const char *email,
                                          const char *country, bool is_artist,
                                          const char *sub_type, bool auto_renewal,
                                          int dias_validez)
{
    json_object *u = json_object_new_object();

    json_object_object_add(u, "name", json_object_new_string(name));
    json_object_object_add(u, "username", json_object_new_string(username));
    json_object_object_add(u, "email", json_object_new_string(email));
    json_object_object_add(u, "country", json_object_new_string(country));
    json_object_object_add(u, "isArtist", json_object_new_boolean(is_artist));

    json_object *subscription = json_object_new_object();
    json_object_object_add(subscription, "type", json_object_new_string(sub_type));
    json_object_object_add(subscription, "autoRenewal", json_object_new_boolean(auto_renewal));

    time_t now = time(NULL);
    now += dias_validez * 24 * 60 * 60;
    struct tm *exp_date = localtime(&now);
    char fecha_exp[11];
    strftime(fecha_exp, sizeof(fecha_exp), "%Y-%m-%d", exp_date);
    json_object_object_add(subscription, "expiration", json_object_new_string(fecha_exp));

    json_object_object_add(u, "subscription", subscription);

    return u;
}

static message_handler(get_user_data)
{
    if (!usuario)
    {
        VibeCast_SendBool(id, HTTP_UNAUTHORIZED, false, "No hay sesión activa", STATE_FAILURE);
        return;
    }

    bool esArtista = usuario->artista ? true : false;
    const char *plan = (usuario->plan == PLAN_PREMIUM) ? "premium" : "freemium";

    json_object *u = exportar_usuario_json(
        usuario->nickname,
        usuario->username,
        usuario->email,
        usuario->pais,
        esArtista,
        plan,
        true,
        180);

    VibeCast_SendJSON(id, HTTP_OK, u, "Datos del usuario cargados", STATE_SUCCESS);
    json_object_put(u);

    puts("Información de perfil enviada");
}

/* ================================================================ */

static message_handler(get_canciones)
{
    puts("Obteniendo canciones");
    VibeCast_SendArray(id, HTTP_OK, NULL, "Canciones cargadas", STATE_SUCCESS);
}

static message_handler(get_recomendaciones)
{
    puts("Obteniendo recomendaciones");
    VibeCast_SendArray(id, HTTP_OK, NULL, "Recomendaciones cargadas", STATE_SUCCESS);
}

/* ======== Para conectar las interfaces ======== */

bool VibeCast_InitBindings()
{
    char *msg = NULL;

    bind_fn(test, &msg);
    bind_fn(is_logged_in, &msg);
    bind_fn(iniciar_sesion, &msg);
    bind_fn(cerrar_sesion, &msg);
    bind_fn(crear_cuenta, &msg);
    bind_fn(next_song, &msg);
    bind_fn(get_user_data, &msg);
    bind_fn(get_canciones, &msg);
    bind_fn(get_recomendaciones, &msg);

    return true;
}
