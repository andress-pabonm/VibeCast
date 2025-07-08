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

    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    // Verificar si el usuario ya es amigo
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);

    if (amigo)
    {
        send_message("El usuario %s ya es tu amigo.\n", amigo->username);
        return false;
    }

    // Buscar usuario en el sistema global
    amigo = searchValueInABB(usuarios, username, cmpUsuarioConUsername);

    if (!amigo)
    {
        send_message("El usuario %s no existe en el sistema\n", username);
        return false;
    }

    // Agregar a lista de amigos
    insertValueInLista(amigos, amigo);
    send_message("El usuario %s ha sido agregado a tu lista de amigos\n", amigo->username);

    char *values = asprintf("%d, %d", usuario->id);

    nuevo_registro(
        "Amigos", "id_usuario_1, id_usuario_2", values, NULL);
}

interfaz(EliminarAmigo)
{
    // Obtener el username a eliminar
    const char *username = argv[0];

    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    // Eliminar de la lista de amigos
    Usuario *amigo = deleteValueInLista(amigos, username, cmpUsuarioConUsername);

    if (amigo)
        send_message("El usuario %s ha sido eliminado de tu lista de amigos\n", amigo->username);
    else
        send_message("El usuario %s no está en tu lista de amigos.\n", username);
}

json_object *amigo_to_json(Usuario *amigo)
{
    // Creo un nuevo objeto JSON
    json_object *jobj = json_object_new_object();

    // Rellenar los campos necesarios para mostrar el amigo en la interfaz gráfica
    // Revisar "../../ui/Amigos/amigosApp.js"

    json_object_object_add(
        jobj, 
        "nombre",
        json_object_new_string(amigo->nickname));

         json_object_object_add(
        jobj, 
        "id",
        json_object_new_string(amigo->id));
    // Añadir el resto de campos

    /*
    {   
        "nombre": "nickname"
        "id": "12345",
    }
     */

    return jobj;
}

new_operfn(obtenerAmigo)
{
    json_object_array_add(arg, amigo_to_json(val));
    return FOREACH_CONTINUE;
}

message_handler(mostrar_amigos)
{
    // Crear un array para los amigos
    json_object *array = json_object_new_array();

    // Recorrer la lista de amigos para agregarlos a (array)
    forEachInLista(usuario->amigos, obtenerAmigo, array);

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

// void mostrarAmigos()
// {
//     // Obtener lista de amigos del usuario actual
//     Lista amigos = usuario->amigos;

//     if (!amigos)
//     {
//         printf("No tienes amigos agregados\n");
//         return;
//     }

//     printf("Tus amigos:\n");
//     forEachInLista(amigos, mostrar_amigos, NULL); // Imprimir cada amigo en la lista
// }

Lista recomendarCanciones()
{
    // Crear una lista para las canciones a recomendar
    Lista listaCancionesRecomendadas = newLista(NULL); // Crear una nueva lista para las canciones recomendadas

    // Obtener las canciones del historial de cada amigo
    forEachInLista(usuario->amigos, obtenerHistorialDeUsuario, listaCancionesRecomendadas); // Obtenemos la lista de recomendaciones de amigos

    // Retornar la lista de recomendaciones
    return listaCancionesRecomendadas;
}


