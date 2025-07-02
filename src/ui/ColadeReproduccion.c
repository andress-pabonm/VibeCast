#include <ui/interfaces.h>
#include <utils/utils.h>

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

static void encolar(void *dato, ElementoColaTipo tipo)
{
    NodoColaRepr nodo = alloc(struct NodoColaRepr, NULL);
    if (!nodo)
        return;

    nodo->dato = dato;
    nodo->tipo = tipo;

    insertValueInCola(cola_repr, nodo);
}

void agregarCancionACola(Cancion *cancion)
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
}

interfaz(SiguienteCancion)
{
    struct
    {
        Cancion *cancion;
        Anuncio *anuncio;
    } *elm = arg;

    NodoColaRepr nodo = deleteValueInCola(cola_repr);

    switch (nodo->tipo)
    {
    case TIPO_CANCION:
        elm->cancion = nodo->dato;
        break;

    case TIPO_ANUNCIO:
        elm->anuncio = nodo->dato;
        break;

    default:
        puts("Tipo de dato inseperado.");
    }

    freem(nodo);

    return true;
}
