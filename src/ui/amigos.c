#include <ui/interfaces.h>
#include <db/dbmgr.h>

// new_operfn(mostrarAmigo)
// {
//     Usuario *amigo = val;                      // Puntero al usuario amigo actual
//     printf("%d): %s\n", idx, amigo->username); // Muestra el índice y el nombre de usuario del amigo

//     return FOREACH_CONTINUE; // Continúa el bucle
// }

new_operfn(obtenerHistorialDeUsuario)
{
    Usuario *amigo = val;                   // Es un puntero a usuario que esta en la lista de amigos
    Lista ListaCancionesRecomendadas = arg; // Argumento para la funcion de reccorrer la lista de amigos

    Pila historial = amigo->historial.reproducciones;
    Pila TempHistorial = newPila();

    // Obtenemos la pila de reproducciones del historial del usuario
    Cancion *canciontemp;
    Reproduccion *reproducciontemp = deleteValueInPila(historial);

    while (reproducciontemp)
    {
        // Recorremos la pila de reproducciones del usuario
        if (!searchValueInLista(ListaCancionesRecomendadas, &reproducciontemp->cancion->id, cmpCancionConId))
        {
            canciontemp = searchValueInLista(canciones, &reproducciontemp->cancion->id, cmpCancionConId);
            insertValueInLista(ListaCancionesRecomendadas, canciontemp);
        }

        insertValueInPila(TempHistorial, reproducciontemp); // Insertamos la reproduccion en la pila temporal
        reproducciontemp = deleteValueInPila(historial);    // Obtenemos la siguiente reproduccion
    }

    reproducciontemp = deleteValueInPila(TempHistorial); // Obtenemos la ultima reproduccion

    while (reproducciontemp)
    {
        insertValueInPila(historial, reproducciontemp);
        reproducciontemp = deleteValueInPila(TempHistorial); // Insertamos las reproducciones de la pila temporal en la pila de reproducciones del historial
    }

    destroyPila(TempHistorial, NULL, NULL); // Destruimos la pila temporal

    return FOREACH_CONTINUE; // Continuar recorriendo la lista de amigos
}

interfaz(AgregarAmigo)
{
    // Obtener el username
    const char *username = argv[0];

    // Validar que no sea el mismo usuario
    if (!strcmp(usuario->username, username))
    {
        send_message("No puedes agregarte a ti mismo como amigo.");
        return false;
    }

    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    // Verificar si el usuario ya es amigo
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);

    if (amigo)
    {
        send_message("El usuario '%s' ya es tu amigo.\n", username);
        return false;
    }

    // Buscar usuario en el sistema global
    amigo = searchValueInABB(usuarios, username, cmpUsuarioConUsername);

    if (!amigo)
    {
        send_message("El usuario '%s' no existe.", username);
        return false;
    }

    // Agregar en la base de datos
    char *values = asprintf("%d, %d", usuario->id, amigo->id);
    if (!nuevo_registro("Amigos", "id_usuario_1, id_usuario_2", values, msg))
    {
        freem(values);
        return false;
    }
    freem(values);

    // Agregar a lista de amigos
    insertValueInLista(amigos, amigo);
    send_message("El usuario '%s' ha sido agregado a tu lista de amigos.", username);

    return true;
}

interfaz(EliminarAmigo)
{
    // Obtener el username a eliminar
    const char *username = argv[0];

    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    // Eliminar de la lista de amigos
    Usuario *amigo = deleteValueInLista(amigos, username, cmpUsuarioConUsername);

    if (!amigo)
    {
        send_message("El usuario '%s' no está en tu lista de amigos.", username);
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

    send_message("El usuario '%s' ha sido eliminado de tu lista de amigos.", username);

    return true;
}

json_object *amigo_to_json(Usuario *amigo)
{
    // Creo un nuevo objeto JSON
    json_object *jobj = json_object_new_object();

    json_object_object_add(
        jobj,
        "id",
        json_object_new_int(amigo->id));

    json_object_object_add(
        jobj,
        "nombre",
        json_object_new_string(amigo->nickname));

    return jobj;
}

new_operfn(getAmigos)
{
    json_object_array_add(arg, amigo_to_json(val));
    return FOREACH_CONTINUE;
}

message_handler(obtener_amigos)
{
    // Crear un array para los amigos
    json_object *array = json_object_new_array();

    // Recorrer la lista de amigos para agregarlos a (array)
    forEachInLista(usuario->amigos, getAmigos, array);

    size_t len = json_object_array_length(array);

    // Enviar la lista a la interfaz gráfica
    VibeCast_SendArray(
        id,
        (len > 0)
            ? HTTP_OK
            : HTTP_NO_CONTENT,
        array,
        "Amigos cargados",
        STATE_SUCCESS);

    // Liberar la memoria
    json_object_put(array);
}

message_handler(agregar_amigo)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char **argv =
        cast(const char *[],
             username);

    char **msg = arg;

    bool success = VibeCast_AgregarAmigo(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}

message_handler(eliminar_amigo)
{
    init_data_json();

    const char *username = get_string(get_array_idx(data, 0));

    int argc = 1;
    const char **argv =
        cast(const char *[],
             username);

    char **msg = arg;

    bool success = VibeCast_EliminarAmigo(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    freem(*msg);
    *msg = NULL;

    end_data_json();
}
