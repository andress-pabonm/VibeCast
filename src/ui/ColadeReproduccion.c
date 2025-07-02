#include <ui/interfaces.h>
#include <utils/utils.h>

// AQUI ESTABAN TODAS LAS ESTRUCTURAS DE ESTRUCTURAS.H

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
    // LO QUE PASA ES QUE EN LA FUNCION ENCOLAR, EL PRIMER PARAMETRO ES DE TIPO COLA, PERO AQUI LO MANDAN
    // COMO OTRA TIPO DE ESTRUCTURA QUE ES NodoColadeReproduccion, PERO NO DA ERROR, COMO NODOCOLADEREPRODUCCION
    // TAMBIEN TIENE PUNTEROS DE FRENTE Y FINAL, ENTONCES EL DEEPSEEP ME DIJO Q ES UN TIPO DE DISCREPANCIA, NO SE Q SIGNIFICA DISCREPANCIA XD
    // PERO UN PANA ME DIJO Q PUEDE SER POLIMORFISMO, YA QUE VI LA ESTRUCTURA EN LOS .H Y PUES TAMBIEN TIENE QUE SE CREA EL NODO
    // COMO LA ESTRUCCUTRA NODOCOLA, Y LUEGO LE UNE COMO COLA LA ESTRUCTURA NodoColadeReproduccion, NO SE BRO NO ENTIENDO JAJAJAJ
    // INTENTE UNIR PERO ESTOY ENTENDIENDO XQ NO DA ERROR ENTONCES NO SE QUE MISMO PASA, POR ESO MIMSO NO HE SEGUIDO EN LA UNION DEL CODIGO,
    // PERO AHI VELE Y DAME TUS CONCLUSIONES, IGUAL ESO PIENSO, SI DIJE ALGUNA ESTUPIDEZ DISCULPARAS XD PERO ESO MAS O MENOS ENTENDI Y ME DIJIERON JAJA

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

// NodoCola* desencolar(NodoColadeReproduccion *cola) {

//     if (cola->frente == NULL) return NULL; // Cola vacía

//     NodoCola* nodo = cola->frente;

//     deleteValueInCola(nodo);

//     // cola->frente = nodo->siguiente;

//     // // Si la cola queda vacía, actualiza también el final
//     // if (cola->frente == NULL) {
//     //     cola->final = NULL;
//     // }

//     // nodo->siguiente = NULL; // Desvincula el nodo de la cola
//     return nodo;
// }

// void reproducirCola(NodoColadeReproduccion* cola) {
//     if (cola->frente==NULL){
//         printf("La cola de reproducción está vacía\n");
//         return;
//     }

//     // 1. Bucle para procesar toda la cola.
//     while (cola->frente !=NULL) {
//         // Sacamos el primer nodo
//         NodoCola* nodoActual = desencolar(cola);

//         // 2. Miramos la etiqueta para saber qué hacer.
//         if (nodoActual->tipo == TIPO_CANCION) {
//             // 3. Es una canción: casteamos el puntero y lo procesamos.
//             Cancion* cancion = (Cancion*)nodoActual->dato;
//             printf("Reproduciendo: '%s' ", cancion->nombre);

//         } else if (nodoActual->tipo == TIPO_ANUNCIO) {
//             // 3. Es un anuncio: casteamos el puntero y lo procesamos.
//             Anuncio* anuncio = (Anuncio*)nodoActual->dato;
//             printf("ANUNCIO: %s (%s)\n", anuncio->url, anuncio->anunciante);
//         }

//         // 4. Liberamos la memoria de la "caja" (el nodo), no del contenido.
//         free(nodoActual);
//     }

//     printf("--- FIN DE LA REPRODUCCIÓN ---\n");
// }

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
