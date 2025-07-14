#include <ui/interfaces.h>

/* ================================================================ */
// DECLARACIONES DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(ObtenerAmigos);
static new_operfn(getAmigoJSON);

static interfaz(AgregarAmigo);

static interfaz(EliminarAmigo);
static new_cmpfn(cmpUsuarioConId);

/* ================================================================ */
// BLOQUE: obtener_amigos — Obtener la lista de amigos
/* ================================================================ */

message_handler(obtener_amigos)
{
    json_object *friends_array = json_object_new_array();
    char **msg = arg;

    bool success = VibeCast_ObtenerAmigos(friends_array, 0, NULL, msg);
    VibeCast_SendArray(id, HTTP_OK, friends_array, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;
    json_object_put(friends_array);
}

static interfaz(ObtenerAmigos)
{
    forEachInLista(usuario->amigos, getAmigoJSON, arg);

    if (json_object_array_length(arg))
        send_message("Amigos cargados.");
    else
        send_message("Tu lista de amigos está vacia.");

    return true;
}

static new_operfn(getAmigoJSON)
{
    Usuario *amigo = val;

    // Convertir a formato JSON
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "id", json_object_new_int(amigo->id));
    json_object_object_add(jobj, "nombre", json_object_new_string(amigo->nickname));

    // Insertarlo en la lista JSON
    json_object_array_add(arg, jobj);

    return FOREACH_CONTINUE;
}

/* ================================================================ */
// BLOQUE: agregar_amigo — Agregar un nuevo amigo
/* ================================================================ */

message_handler(agregar_amigo)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {username};
    char **msg = arg;

    bool success = VibeCast_AgregarAmigo(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(AgregarAmigo)
{
    const char *username = argv[0];

    if (!strcmp(usuario->username, username))
    {
        send_message("No puedes agregarte a ti mismo como amigo.");
        return false;
    }

    Lista amigos = usuario->amigos;
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);

    if (amigo)
    {
        send_message("El usuario '%s' ya es tu amigo.\n", username);
        return false;
    }

    amigo = searchValueInABB(usuarios, username, cmpUsuarioConUsername);

    if (!amigo)
    {
        send_message("El usuario '%s' no existe.", username);
        return false;
    }

    char *values = asprintf("%d, %d", usuario->id, amigo->id);
    if (!nuevo_registro("Amigos", "id_usuario_1, id_usuario_2", values, msg))
    {
        freem(values);
        return false;
    }
    freem(values);

    insertValueInLista(amigos, amigo);
    send_message("El usuario '%s' ha sido agregado a tu lista de amigos.", username);

    return true;
}

/* ================================================================ */
// BLOQUE: eliminar_amigo — Eliminar un amigo existente
/* ================================================================ */

message_handler(eliminar_amigo)
{
    init_data_json();

    const char *id_usuario = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char *argv[] = {id_usuario};
    char **msg = arg;

    bool success = VibeCast_EliminarAmigo(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(EliminarAmigo)
{
    int id = atoi(argv[0]);

    Lista amigos = usuario->amigos;
    Usuario *amigo = deleteValueInLista(amigos, &id, cmpUsuarioConId);

    if (!amigo)
    {
        send_message("No se encontró al usuario en tu lista de amigos.");
        return false;
    }

    char *condition = asprintf("id_usuario_1 = %d AND id_usuario_2 = %d", usuario->id, amigo->id);
    if (!eliminar_registros("Amigos", condition, msg))
    {
        insertValueInLista(amigos, amigo);
        freem(condition);
        return false;
    }
    freem(condition);

    send_message("Amigo eliminado.");

    return true;
}

static new_cmpfn(cmpUsuarioConId)
{
    const Usuario *u = val_1;
    const int *id = val_2;

    return u->id - *id;
}
