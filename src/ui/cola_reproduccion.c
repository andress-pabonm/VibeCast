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
} NodoColaRepr;

/* ================================================================ */
// DECLARACIÓN DE FUNCIONES INTERNAS
/* ================================================================ */

static interfaz(Encolar);
static bool encolarNodo(void *dato, ElementoColaTipo tipo);

static interfaz(Decolar);
static bool agregarAHistorial(void *dato, ElementoColaTipo tipo);

static interfaz(VaciarCola);

static NodoColaRepr *newNodoColaRepr();
static void destroyNodoColaRepr(NodoColaRepr *nodo);
static new_operfn(destroyNodosColaRepr);

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

    int argc = 1;
    const char *argv[] = {id_cancion};
    char **msg = arg;

    bool success = VibeCast_Encolar(NULL, argc, argv, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;

    end_data_json();
}

static interfaz(Encolar)
{
    static int cant_canciones = 0;
    int id_cancion = atoi(argv[0]);

    Cancion *cancion = searchValueInLista(canciones, &id_cancion, cmpCancionConId);
    if (!cancion)
    {
        send_message("No se ha encontrado la canción.");
        return false;
    }

    if (!encolarNodo(cancion, TIPO_CANCION))
    {
        send_message("No fue posible encolar la canción.");
        return false;
    }

    cant_canciones = (cant_canciones + 1) % 3;
    send_message("Canción '%s' agregada a la cola.", cancion->nombre);

    if (!cant_canciones && usuario->plan == PLAN_FREEMIUM)
    {
        Anuncio *anuncio = deleteValueInCola(anuncios);

        if (!anuncio)
            puts("No hay anuncios disponibles.");
        else if (!encolarNodo(anuncio, TIPO_ANUNCIO))
            puts("No fue posible encolar un anuncio.");
    }

    return true;
}

static bool encolarNodo(void *dato, ElementoColaTipo tipo)
{
    if (!dato)
        return false;

    NodoColaRepr *nodo = newNodoColaRepr();
    if (!nodo)
        return false;

    nodo->dato = dato;
    nodo->tipo = tipo;

    if (!cola_repr)
        cola_repr = newCola();
    insertValueInCola(cola_repr, nodo);

    return true;
}

/* ================================================================ */
// BLOQUE: decolar — Reproducir siguiente elemento
/* ================================================================ */

message_handler(decolar)
{
    json_object *jobj = json_object_new_object();
    char **msg = arg;

    bool success = VibeCast_Decolar(jobj, 0, NULL, msg);
    VibeCast_SendJSON(id, HTTP_OK, jobj, *msg, STATE_BOOL(success));

    puts(*msg);
    freem(*msg);
    *msg = NULL;

    json_object_put(jobj);
}

static interfaz(Decolar)
{
    if (!cola_repr)
        cola_repr = newCola();
    NodoColaRepr *nodo = deleteValueInCola(cola_repr);

    if (!nodo)
    {
        send_message("La cola está vacia.");
        return false;
    }

    void *dato = nodo->dato;
    ElementoColaTipo tipo = nodo->tipo;
    destroyNodoColaRepr(nodo);

    if (!dato)
    {
        send_message("Elemento nulo en la cola.");
        return false;
    }

    json_object *title;
    json_object *artist;
    json_object *url;

    switch (nodo->tipo)
    {
    case TIPO_CANCION:
        Cancion *cancion = dato;

        title = json_object_new_string(cancion->nombre);
        artist = json_object_new_string(cancion->album->artista->nombre);
        url = json_object_new_string(cancion->url);

        send_message("Reproduciendo canción '%s'.", cancion->nombre);
        break;

    case TIPO_ANUNCIO:
        Anuncio *anuncio = nodo->dato;

        title = json_object_new_string("Anuncio publicitario");
        artist = json_object_new_string(anuncio->anunciante->nickname);
        url = json_object_new_string(anuncio->url);

        send_message("Anuncio publicitario.");
        break;

    default:
        send_message("Elemento desconocido en la cola.");
        return false;
    }

    json_object_object_add(arg, "title", title);
    json_object_object_add(arg, "artist", artist);
    json_object_object_add(arg, "url", url);

    if (!agregarAHistorial(dato, tipo))
        puts("No fue posible agregar al historial.");

    return true;
}

static bool agregarAHistorial(void *dato, ElementoColaTipo tipo)
{
    if (!dato)
        return false;

    datetime_buf_t buf;
    getDateTime(buf, now());

    switch (tipo)
    {
    case TIPO_CANCION:
        Reproduccion *repr = newReproduccion();
        if (!repr)
            return false;

        char *values = asprintf(stringify("%d", "%d", "%s"), usuario->id, cast(Cancion *, dato)->id, buf);
        bool ok = nuevo_registro("Reproducciones", "id_usuario, id_cancion, fecha_escuchado", values, NULL);
        freem(values);

        if (!ok)
            return false;

        repr->cancion = dato;
        repr->fechaEscuchado = asprintf(buf);

        Cancion *cancion = dato;
        cancion->reproducciones++;

        values = asprintf("reprducciones = %d", cancion->reproducciones);
        char *condition = asprintf("id = %d", cancion->id);
        actualizar_registros("Canciones", values, condition, NULL);
        freem(values);
        freem(condition);

        usuario->historial.tiempoEscuchado += cancion->duracion;

        values = asprintf("tiempo_escuchado = %d", usuario->historial.tiempoEscuchado);
        char *condition = asprintf("id = %d", usuario->id);
        actualizar_registros("Usuarios", values, condition, NULL);
        freem(values);
        freem(condition);

        return insertValueInPila(usuario->historial.reproducciones, repr);

    case TIPO_ANUNCIO:
        usuario->historial.cantidadAnuncios++;

        values = asprintf("cantidad_anuncios = %d", usuario->historial.cantidadAnuncios);
        condition = asprintf("id = %d", usuario->id);
        ok = actualizar_registros("Usuarios", values, condition, NULL);
        freem(values);
        freem(condition);

        Anuncio *anuncio = dato;
        condition = asprintf("id = %d", anuncio->id);
        eliminar_registros("Anuncios", condition, NULL);
        freem(anuncio);
        destroyAnuncio(anuncio);

        return ok;

    default:
        return false;
    }
}

/* ================================================================ */
// BLOQUE: vaciar_cola — Vaciar la cola de reproducción
/* ================================================================ */

message_handler(vaciar_cola)
{
    char **msg = arg;

    bool success = VibeCast_VaciarCola(NULL, 0, NULL, msg);
    VibeCast_SendNull(id, HTTP_OK, *msg, STATE_BOOL(success));

    puts(*msg);

    freem(*msg);
    *msg = NULL;
}

static interfaz(VaciarCola)
{
    destroyCola(cola_repr, destroyNodosColaRepr, NULL);
    cola_repr = NULL;

    send_message("Cola de reproducción vaciada.");
    return true;
}

/* ================================================================ */
// BLOQUE: Funciones auxiliares para NodoColaRepr
/* ================================================================ */

static NodoColaRepr *newNodoColaRepr()
{
    return alloc(NodoColaRepr, NULL);
}

static void destroyNodoColaRepr(NodoColaRepr *nodo)
{
    freem(nodo);
}

static new_operfn(destroyNodosColaRepr)
{
    destroyNodoColaRepr(val);
    return FOREACH_CONTINUE;
}
