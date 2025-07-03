#include <ui/interfaces.h>
#include <utils/utils.h>
#include <time.h>

/* ======== Utilidades ======== */

// JSON-C helpers
#define get_json json_tokener_parse
#define get_array_idx json_object_array_get_idx

static inline json_object *get_field(json_object *src, const char *field)
{
    json_object *dest;
    json_object_object_get_ex(src, field, &dest);
    return dest;
}

#define init_json(dest, src) json_object *dest = src
#define init_data_json() init_json(data, get_json(req))
#define get_string(src) json_object_get_string(src)

/* ================================ Interfaces ================================ */

static message_handler(test)
{
    puts(req);
    VibeCast_SendText(id, HTTP_OK, "", "Test ejecutado", STATE_SUCCESS);
}

static message_handler(is_logged_in)
{
    bool result = VibeCast_IsLoggedIn(NULL, 0, NULL, NULL);
    VibeCast_SendBool(id, result ? HTTP_OK : HTTP_UNAUTHORIZED, result, NULL, STATE_SUCCESS;
}

static message_handler(iniciar_sesion)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0));
    const char *password = get_string(get_array_idx(data, 1));

    char **msg = arg;
    bool success = VibeCast_IniciarSesion(NULL, 2, cast(const char *[], username, password), msg);

    VibeCast_SendBool(id, success ? HTTP_OK : HTTP_BAD_REQUEST, success, *msg, success ? STATE_SUCCESS : STATE_FAILURE);

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

static message_handler(cerrar_sesion)
{
    bool success = VibeCast_CerrarSesion(NULL, 0, NULL, NULL);
    VibeCast_SendBool(id, success ? HTTP_OK : HTTP_BAD_REQUEST, success,
                      success ? "Sesión cerrada" : "No se pudo cerrar sesión",
                      success ? STATE_SUCCESS : STATE_FAILURE);
}

static message_handler(crear_cuenta)
{
    init_data_json();

    const char *nickname = get_string(get_array_idx(data, 0));
    const char *pais = get_string(get_array_idx(data, 1));
    const char *username = get_string(get_array_idx(data, 2));
    const char *email = get_string(get_array_idx(data, 3));
    const char *password = get_string(get_array_idx(data, 4));
    const char *confirmPassword = get_string(get_array_idx(data, 5));

    const char *datos[] = {email, username, password, confirmPassword, nickname, pais};

    char **msg = arg;
    bool success = VibeCast_CrearCuenta(NULL, 6, datos, msg);

    VibeCast_SendBool(id, success ? HTTP_CREATED : HTTP_BAD_REQUEST, success, *msg, success ? STATE_SUCCESS : STATE_FAILURE);

    puts(*msg);
    freem(*msg);
    *msg = NULL;
}

/* ================================================================ */

static message_handler(next_song)
{
    struct
    {
        Cancion *cancion;
        Anuncio *anuncio;
    } elm = {NULL, NULL};

    VibeCast_SiguienteCancion(&elm, 0, NULL, NULL);

    if (elm.cancion)
    {
        // json_object *c = exportar_cancion_json(elm.cancion);
        // VibeCast_SendJSON(id, HTTP_OK, c, NULL, STATE_SUCCESS);
        // json_object_put(c);
    }
    else if (elm.anuncio)
    {
        // ...
    }
    else
    {
        VibeCast_SendText(id, HTTP_NO_CONTENT, "", "No hay más canciones o anuncios", STATE_SUCCESS);
    }
}

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
