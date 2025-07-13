#include <ui/interfaces.h>

/* ================================================================ */
// DEFINICIÓN DE ESTRUCTURAS INTERNAS
/* ================================================================ */

typedef enum
{
    TIPO_CANCION,
    TIPO_ANUNCIO
} ElementoColaTipo;

typedef struct NodoColaRepr
{
    void *dato;            // Puntero genérico: Cancion o Anuncio
    ElementoColaTipo tipo; // Etiqueta del tipo de dato
} *NodoColaRepr;

/* ================================================================ */
// DECLARACIÓN DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(Encolar);
static interfaz(Decolar);
static interfaz(VaciarCola);

static interfaz(AgregarCancionACola);
static void agregarCancionACola(Cancion *cancion);
static void agregarACola(void *dato, ElementoColaTipo tipo);

static interfaz(VaciarColaReproduccion);

/* ================================================================ */
// VARIABLE GLOBAL DE COLA DE REPRODUCCIÓN
/* ================================================================ */

Cola cola_repr = NULL;

/* ================================================================ */
// BLOQUE: encolar — Agregar canción a la cola
/* ================================================================ */

message_handler(encolar)
{
    init_data_json();
    const char *id_cancion = get_string(get_array_idx(data, 0));

    const char *argv[] = {id_cancion};
    char **msg = arg;
    bool ok = VibeCast_AgregarCancionACola(NULL, 1, argv, msg);

    VibeCast_SendNull(id, ok ? HTTP_CREATED : HTTP_BAD_REQUEST, *msg, STATE_BOOL(ok));

    freem(*msg);
    *msg = NULL;

    end_data_json();

    puts("Que rico el sexo lesbico la plenaaaa\nCancion encolada\n");
}

static interfaz(AgregarCancionACola)
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

static void agregarCancionACola(Cancion *cancion)
{
    static int cant_canciones = 0;

    agregarACola(cancion, TIPO_CANCION);
    cant_canciones++;

    if (cant_canciones == 3)
    {
        if (usuario->plan == PLAN_FREEMIUM)
        {
            Anuncio *anuncio = deleteValueInCola(anuncios);
            agregarACola(anuncio, TIPO_ANUNCIO);
        }

        cant_canciones = 0;
    }
}

static void agregarACola(void *dato, ElementoColaTipo tipo)
{
    NodoColaRepr nodo = alloc(struct NodoColaRepr, NULL);
    if (!nodo)
        return;

    nodo->dato = dato;
    nodo->tipo = tipo;

    if (!cola_repr)
        cola_repr = newCola();

    insertValueInCola(cola_repr, nodo);
}

/* ================================================================ */
// BLOQUE: decolar — Reproducir siguiente elemento
/* ================================================================ */

message_handler(decolar)
{
    if (!cola_repr)
        cola_repr = newCola();

    NodoColaRepr nodo = deleteValueInCola(cola_repr);

    if (!nodo)
    {
        VibeCast_SendNull(id, HTTP_OK, "No hay más canciones en la cola.", STATE_SUCCESS);
        return;
    }

    json_object *jobj = new_json_object();

    Cancion *c = NULL;
    Anuncio *a = NULL;

    switch (nodo->tipo)
    {
    case TIPO_CANCION:
        c = nodo->dato;
        json_object_object_add(jobj, "title", json_object_new_string(c->nombre));
        json_object_object_add(jobj, "artist", json_object_new_string(c->album->artista->nombre));
        json_object_object_add(jobj, "url", json_object_new_string(c->url));
        break;

    case TIPO_ANUNCIO:
        a = nodo->dato;
        json_object_object_add(jobj, "title", json_object_new_string("Anuncio publicitario"));
        json_object_object_add(jobj, "artist", json_object_new_string(a->anunciante->nickname));
        json_object_object_add(jobj, "url", json_object_new_string(a->url));
        break;

    default:
        break;
    }

    agregarAHistorial(usuario, c, a);

    freem(nodo);

    VibeCast_SendJSON(id, HTTP_OK, jobj, "Siguiente canción", STATE_SUCCESS);
    json_object_put(jobj);
}

/* ================================================================ */
// BLOQUE: vaciar_cola — Vaciar la cola de reproducción
/* ================================================================ */

message_handler(vaciar_cola)
{
    VibeCast_VaciarColaReproduccion(NULL, 0, NULL, NULL);
    VibeCast_SendNull(id, HTTP_OK, "Cola vaciada", STATE_SUCCESS);
}

static interfaz(VaciarColaReproduccion)
{
    destroyCola(cola_repr, NULL, NULL);
    cola_repr = NULL;
    send_message("Cola de reproducción vaciada.");

    return true;
}
