#include <ui/interfaces.h>

new_operfn(ObtenerHistorial)
{
  Usuario *u = val;                       // val=void *val, que es un puntero a Usuario que estan en la lista de amigos
  Lista ListaCancionesRecomendadas = arg; // argumento para la funcion de reccorrer la lista de amigos

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

Lista recomendarCanciones()
{
  Lista ListaCancionesRecomendadas = newLista(NULL); // Crear una nueva lista para las canciones recomendadas
  forEachInLista(usuario->amigos, ObtenerHistorial, ListaCancionesRecomendadas);
  // recorrer la lista de amigos y llamar a la funcion recomendar_canciones
  return ListaCancionesRecomendadas; // Retornar la lista de canciones recomendadas
}

message_handler(get_recomendaciones)
{
  json_object *array = json_object_new_array();
  VibeCast_SendArray(id, HTTP_OK, array, "Recomendaciones cargadas", STATE_SUCCESS);
  json_object_put(array);
}