#include <ui/interfaces.h>

Cola cola_repr = NULL;

/* ================================================================ */

typedef enum
{
    TIPO_CANCION,
    TIPO_ANUNCIO
} ElementoColaTipo;

typedef struct NodoColaRepr
{
    void *dato;            // Puntero GENÉRICO. Apuntará a una CancionGobal o a un Anuncio.
    ElementoColaTipo tipo; // La ETIQUETA que nos dice qué es 'dato'.
} *NodoColaRepr;

/* ================================================================ */

message_handler(next_song)
{
    if (!cola_repr)
        cola_repr = newCola();

    NodoColaRepr nodo = deleteValueInCola(cola_repr);

    if (!nodo)
    {
        VibeCast_SendNull(
            id,
            HTTP_NO_CONTENT,
            "La cola está vacia",
            STATE_FAILURE);
        return;
    }

    switch (nodo->tipo)
    {
    case TIPO_CANCION:
        break;

    case TIPO_ANUNCIO:
        break;

    default:
        puts("Tipo de dato inseperado.");
    }

    freem(nodo);
}

/* ================================================================ */

static void encolar(void *dato, ElementoColaTipo tipo)
{
    NodoColaRepr nodo = alloc(struct NodoColaRepr, NULL);
    if (!nodo)
        return;

    nodo->dato = dato;
    nodo->tipo = tipo;

    insertValueInCola(cola_repr, nodo);
}

bool agregarCancionACola(Cancion *cancion)
{
    static int cant_canciones = 0;

    // 1. Siempre se agrega la canción a la cola.
    // Se usa la función 'encolar' interna, pasándole la canción y su TIPO.
    encolar(cancion, TIPO_CANCION);
    cant_canciones++;

    // 2. Se aplica la lógica SOLO para usuarios free.
    if (cant_canciones == 3)
    {
        // Verificar si es FREEMIUM
        if (usuario->plan = PLAN_FREEMIUM)
        {
            // Agregar un anuncio
            Anuncio *anuncio = deleteValueInCola(anuncios);
            encolar(anuncio, TIPO_ANUNCIO);
        }

        cant_canciones = 0;
    }

    return true;
}

/* ================================================================ */

interfaz(SiguienteCancion)
{
    struct
    {
        Cancion *cancion;
        Anuncio *anuncio;
    } *elm = arg;

    return true;
}

interfaz(AgregarCancionACola)
{
    int id = atoi(argv[0]);

    Cancion *c = searchValueInLista(canciones, &id, cmpCancionConId);
    if (!c)
    {
        send_message("La canción con ID %d no existe.", id);
        return false;
    }

    NodoColaRepr nodo = alloc(struct NodoColaRepr, NULL);
    nodo->dato = c;
    nodo->tipo = TIPO_CANCION;

    if (!cola_repr)
        cola_repr = newCola();
    insertValueInCola(cola_repr, nodo);

    send_message("Canción '%s' agregada a la cola.", c->nombre);

    return true;
}

message_handler(enqueue)
{
    init_data_json();
    const char *id_cancion = get_string(get_array_idx(data, 0));

    const char *argv[] = {id_cancion};
    char **msg = arg;
    bool ok = VibeCast_AgregarCancionACola(NULL, 1, argv, msg);

    VibeCast_SendText(id, ok ? HTTP_CREATED : HTTP_BAD_REQUEST, *msg, "Agregar a cola", STATE_BOOL(ok));
    freem(*msg);
    *msg = NULL;
}

interfaz(VaciarColaReproduccion)
{
    destroyCola(cola_repr, NULL, NULL);
    cola_repr = NULL;
    send_message("Cola de reproducción vaciada.");

    return true;
}

message_handler(vaciar_cola)
{
    VibeCast_VaciarColaReproduccion(NULL, 0, NULL, NULL);
    VibeCast_SendText(id, HTTP_OK, "", "Cola vaciada", STATE_SUCCESS);
}

message_handler(mostrar_cola)
{
    Cola temp = newCola();
    json_object *array = json_object_new_array();
    NodoColaRepr nodo;

    while ((nodo = deleteValueInCola(cola_repr)) != NULL)
    {
        json_object *jobj = json_object_new_object();

        if (nodo->tipo == TIPO_CANCION)
        {
            Cancion *c = nodo->dato;
            json_object_object_add(jobj, "tipo", json_object_new_string("cancion"));
            json_object_object_add(jobj, "nombre", json_object_new_string(c->nombre));
            json_object_object_add(jobj, "url", json_object_new_string(c->url));
            json_object_object_add(jobj, "duracion", json_object_new_int(c->duracion));
        }
        else if (nodo->tipo == TIPO_ANUNCIO)
        {
            Anuncio *a = nodo->dato;
            json_object_object_add(jobj, "tipo", json_object_new_string("anuncio"));
            json_object_object_add(jobj, "url", json_object_new_string(a->url));
        }

        json_object_array_add(array, jobj);
        insertValueInCola(temp, nodo); // restaurar
    }

    while ((nodo = deleteValueInCola(temp)) != NULL)
        insertValueInCola(cola_repr, nodo);

    destroyCola(temp, NULL, NULL);

    VibeCast_SendArray(id, json_object_array_length(array) > 0 ? HTTP_OK : HTTP_NO_CONTENT,
                       array, "Cola de reproducción", STATE_SUCCESS);

    json_object_put(array);
}

message_handler(dequeue)
{
    json_object *jobj = new_json_object();

    NodoColaRepr nodo = deleteValueInCola(cola_repr);
    Cancion *c;
    Anuncio *a;

    switch (nodo->tipo)
    {
    case TIPO_CANCION:
        c = nodo->dato;
        json_object_object_add(jobj, "youtubeId", json_object_new_string(c->url));
        break;

    case TIPO_ANUNCIO:
        a = nodo->dato;
        json_object_object_add(jobj, "youtubeId", json_object_new_string(a->url));
        break;

    default:
        break;
    }

    freem(nodo);

    VibeCast_SendJSON(id, HTTP_OK, jobj, "Siguiente canción", STATE_SUCCESS);
    json_object_put(jobj);
}
