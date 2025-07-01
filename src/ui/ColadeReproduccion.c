#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <db/estructuras.h>
 
//AQUI ESTABAN TODAS LAS ESTRUCTURAS DE ESTRUCTURAS.H

typedef enum {
    TIPO_CANCION,
    TIPO_ANUNCIO
} ElementoColaTipo;

// struct NodoAnuncio{
//     // Anuncio publicitario
//     Anuncio *anuncio;
//     // Puntero al siguiente nodo
//     NodoAnuncio *siguiente;
// };

typedef struct NodoCola {
    void *dato;                 // Puntero GENÉRICO. Apuntará a una CancionGobal o a un Anuncio.
    ElementoColaTipo tipo;      // La ETIQUETA que nos dice qué es 'dato'.
}NodoCola;

typedef struct NodoColadeReproduccion {
	NodoCola* frente;  // Apunta al primer elemento (el que saldrá primero).
	NodoCola* final;   // Apunta al último elemento (para agregar nuevos fácilmente).
	int contadorCancionesFree; 
}NodoColadeReproduccion;

void insertarAnuncioCircular(Lista *inicioLista, Anuncio *anuncio) {
    //DOCUMENTADO HECHO POR PAULA Y LEO, INSERTAR ANUNCIOS COMO LISTA CIRCULAR
    // NodoAnuncio *nuevo = (NodoAnuncio *)malloc(sizeof(NodoAnuncio));

    //ANUNCIOS INCERTADOS COMO LISTA 
    insertValueInLista(inicioLista, anuncio);

    // if (!nuevo) return;
    // nuevo->anuncio = anuncio;
    // nuevo->siguiente = NULL;

    // if (*cabeza == NULL) {
    //     // Primer nodo: apunta a sí mismo
    //     nuevo->siguiente = nuevo;
    //     *cabeza = nuevo;
    // } else {
    //     NodoAnuncio *temp = *cabeza;
    //     // Buscar el último nodo (el que apunta a cabeza)
    //     while (temp->siguiente != *cabeza) {
    //         temp = temp->siguiente;
    //     }
    //     temp->siguiente = nuevo;
    //     nuevo->siguiente = *cabeza;
    // }
}

void encolar(Cola *colaReproduccion, void *dato, ElementoColaTipo tipo) {
    //DOCUMENTADO HECHO POR PAULA Y LEO, ENCOLAR
    NodoCola *nuevoNodo = (NodoCola*)malloc(sizeof(NodoCola));

    if (!nuevoNodo) return; // Manejo de error en caso de falla de memoria

    nuevoNodo->dato = dato;
    nuevoNodo->tipo = tipo;

    insertValueInCola(colaReproduccion, nuevoNodo);

    // if (cola->frente == NULL) {
    //     // Cola vacía, el nuevo nodo es tanto el frente como el final
    //     cola->frente = nuevoNodo;
    //     cola->final = nuevoNodo;
    // } else {
    //     // Encolar al final
    //     cola->final->siguiente= nuevoNodo;
    //     cola->final = nuevoNodo;
    // }
}

void agregarCancionACola(NodoColadeReproduccion *colaReproduccion, Usuario* usuario, Cancion *cancion, Anuncio *cabezaAnuncio) {

    //LO QUE PASA ES QUE EN LA FUNCION ENCOLAR, EL PRIMER PARAMETRO ES DE TIPO COLA, PERO AQUI LO MANDAN
    //COMO OTRA TIPO DE ESTRUCTURA QUE ES NodoColadeReproduccion, PERO NO DA ERROR, COMO NODOCOLADEREPRODUCCION
    //TAMBIEN TIENE PUNTEROS DE FRENTE Y FINAL, ENTONCES EL DEEPSEEP ME DIJO Q ES UN TIPO DE DISCREPANCIA, NO SE Q SIGNIFICA DISCREPANCIA XD
    //PERO UN PANA ME DIJO Q PUEDE SER POLIMORFISMO, YA QUE VI LA ESTRUCTURA EN LOS .H Y PUES TAMBIEN TIENE QUE SE CREA EL NODO
    //COMO LA ESTRUCCUTRA NODOCOLA, Y LUEGO LE UNE COMO COLA LA ESTRUCTURA NodoColadeReproduccion, NO SE BRO NO ENTIENDO JAJAJAJ
    //INTENTE UNIR PERO ESTOY ENTENDIENDO XQ NO DA ERROR ENTONCES NO SE QUE MISMO PASA, POR ESO MIMSO NO HE SEGUIDO EN LA UNION DEL CODIGO, 
    //PERO AHI VELE Y DAME TUS CONCLUSIONES, IGUAL ESO PIENSO, SI DIJE ALGUNA ESTUPIDEZ DISCULPARAS XD PERO ESO MAS O MENOS ENTENDI Y ME DIJIERON JAJA


    // 1. Siempre se agrega la canción a la cola.
    // Se usa la función 'encolar' interna, pasándole la canción y su TIPO.
    encolar(colaReproduccion, cancion, TIPO_CANCION);

    // 2. Se aplica la lógica SOLO para usuarios free.
    if (usuario->plan == PLAN_FREEMIUM) {
        colaReproduccion->contadorCancionesFree++; // 3. Se incrementa el contador.

        // 4. Se revisa si es momento de un anuncio.
        if (colaReproduccion->contadorCancionesFree == 3) {
           
            // Se encola el anuncio, pasándole el anuncio y su TIPO.
            encolar(colaReproduccion, cabezaAnuncio, TIPO_ANUNCIO);
            
            //AQUI ANDRES LO QUE DICES XD LA FUNCION
            // cabezaAnuncio=cabezaAnuncio->siguiente; // Avanzamos al siguiente anuncio.
            // colaReproduccion->contadorCancionesFree = 0; // 5. Se reinicia el contador.
        }
    }
}

NodoCola* desencolar(NodoColadeReproduccion *cola) {


    if (cola->frente == NULL) return NULL; // Cola vacía

    NodoCola* nodo = cola->frente;
    
    deleteValueInCola(nodo);

    // cola->frente = nodo->siguiente;

    // // Si la cola queda vacía, actualiza también el final
    // if (cola->frente == NULL) {
    //     cola->final = NULL;
    // }

    // nodo->siguiente = NULL; // Desvincula el nodo de la cola
    return nodo;
}


void reproducirCola(NodoColadeReproduccion* cola) {
    if (cola->frente==NULL){
        printf("La cola de reproducción está vacía\n");
        return;
    }

    // 1. Bucle para procesar toda la cola.
    while (cola->frente !=NULL) {
        // Sacamos el primer nodo
        NodoCola* nodoActual = desencolar(cola);

        // 2. Miramos la etiqueta para saber qué hacer.
        if (nodoActual->tipo == TIPO_CANCION) {
            // 3. Es una canción: casteamos el puntero y lo procesamos.
            Cancion* cancion = (Cancion*)nodoActual->dato;
            printf("Reproduciendo: '%s' ", cancion->nombre);

        } else if (nodoActual->tipo == TIPO_ANUNCIO) {
            // 3. Es un anuncio: casteamos el puntero y lo procesamos.
            Anuncio* anuncio = (Anuncio*)nodoActual->dato;
            printf("ANUNCIO: %s (%s)\n", anuncio->url, anuncio->anunciante);
        }

        // 4. Liberamos la memoria de la "caja" (el nodo), no del contenido.
        free(nodoActual);
    }
    
    printf("--- FIN DE LA REPRODUCCIÓN ---\n");
}
