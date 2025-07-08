#include <ui/interfaces.h>

new_operfn(mostrar_amigos)
{
    Usuario *amigo = val;                      // Puntero al usuario amigo actual
    printf("%d): %s\n", idx, amigo->username); // Muestra el índice y el nombre de usuario del amigo

    return FOREACH_CONTINUE; // Continúa el bucle
}

new_operfn(obtenerHistorial)
{
    Usuario *u = val;                       // Es un puntero a usuario que esta en la lista de amigos
    Lista ListaCancionesRecomendadas = arg; // Argumento para la funcion de reccorrer la lista de amigos

    Pila historial = u->historial.reproducciones;
    Pila TempHistorial = newPila();

    // Obtenemos la pila de reproducciones del historial del usuario
    Cancion *canciontemp;
    Reproduccion *reproducciontemp = deleteValueInPila(historial);

    while (reproducciontemp)
    {
        // Recorremos la pila de reproducciones del usuario
        canciontemp = searchValueInLista(canciones, &reproducciontemp->cancion->id, cmpCancionConId);
        insertValueInLista(ListaCancionesRecomendadas, canciontemp);

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

void agregarAmigo(const char *username)
{
    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    // Verificar si el usuario ya es amigo
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);

    if (amigo)
    {
        printf("El usuario %s ya es tu amigo.\n", amigo->username);
        return;
    }

    // Buscar usuario en el sistema global
    amigo = searchValueInABB(usuarios, username, cmpUsuarioConUsername);

    if (amigo)
    {
        // Agregar a lista de amigos
        insertValueInLista(amigos, amigo);
        printf("El usuario %s ha sido agregado a tu lista de amigos\n", amigo->username);
    }
    else
    {
        printf("El usuario %s no existe en el sistema\n", username);
    }
}

void eliminarAmigo(const char *username)
{
    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    // Buscar usuario en la lista de amigos
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);

    if (amigo)
    {
        // Eliminar de la lista de amigos
        deleteValueInLista(amigos, amigo->username, cmpUsuarioConUsername);
        printf("El usuario %s ha sido eliminado de tu lista de amigos\n", amigo->username);
    }
    else
    {
        printf("El usuario %s no está en tu lista de amigos.\n", username);
    }
}

void mostrarAmigos()
{
    // Obtener lista de amigos del usuario actual
    Lista amigos = usuario->amigos;

    if (!amigos)
    {
        printf("No tienes amigos agregados\n");
        return;
    }

    printf("Tus amigos:\n");
    forEachInLista(amigos, mostrar_amigos, NULL); // Imprimir cada amigo en la lista
}

Lista recomendarCanciones()
{
    Lista listaCancionesRecomendadas = newLista(NULL);                             // Crear una nueva lista para las canciones recomendadas
    forEachInLista(usuario->amigos, obtenerHistorial, listaCancionesRecomendadas); // Obtenemos la lista de recomendaciones de amigos

    return listaCancionesRecomendadas;
}