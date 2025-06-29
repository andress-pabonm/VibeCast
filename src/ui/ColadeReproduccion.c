#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <db/estructuras.h>

#define MAX 100
#define MAX2 20
#define MAX3 200

typedef struct Artista Artista;
typedef struct Album Album;
typedef struct Cancion Cancion;

typedef struct Playlist Playlist;

typedef struct Historial Historial;
typedef struct Reproduccion Reproduccion;

typedef enum Plan Plan;
typedef struct Anuncio Anuncio;

typedef struct Usuario Usuario;
typedef struct NodoAnuncio NodoAnuncio;
typedef struct NodoCancion NodoCancion;
typedef struct NodoPlaylist NodoPlaylist;
typedef struct NodoColadeReproduccion NodoColadeReproduccion;
typedef struct NodoCola NodoCola;

struct Artista
{
	// Puntero a su perfil de usuario
	Usuario *usuario;
	// Nombre del artista
	char *nombre;
	// Lista de los álbumes del artista
	//Lista albumes;
};

// Estructura para representar un álbum de canciones
struct Album
{
	// Identificador único de álbum (utilizado en la base de datos)
	int id;
	// Artista al que pertenece el álbum
	Artista *artista;
	// Nombre del álbum
	char *nombre;
	// Fecha en la que se creó el álbum
	char *fechaCreacion;
	// Lista de canciones del álbum
	//Lista canciones;
};

// Estructura para representar una canción
struct Cancion
{
	// Identificador de la canción
	int id;

	// Álbum al que pertenece la canción
	Album *album;
	// Título de la canción
	char *nombre;
	// Género musical al que pertenece la canción
	char *genero;
	// Fecha en la que se publicó la canción en la app
	char *fechaPublicacion;

	// Duración de la canción
	int duracion;
	// URL donde se encuentra la canción
	char *url;

	// Cantidad de veces que se ha añadido a una playlist
	int popularidad;
	// Cantidad total de reproducciones de la canción
	int reproducciones;

};

// Estructura para representar una playlist
struct Playlist
{
	// Identificador único de playlist (utilizado en la base de datos)
	int id;
	// Nombre de la playlist
	char *nombre;
	// Lista con las canciones agregadas a la playlist
	//Lista canciones;
};


// Estructura para representar el historial de un usuario
struct Historial
{
	// Pila con las reproducciones
	//Pila reproducciones;
	// Tiempo total escuchado/reproducido
	int tiempoEscuchado;
	// Cantidad de anuncios "vistos" por el usuario (PLAN_FREEMIUM)
	int cantidadAnuncios;
};

// Estructura para representar una reproducción de una canción
struct Reproduccion
{
	// Canción reproducida
	Cancion *cancion;
	// Fecha de la reproducción
	char *fechaEscuchado;
};

// Tipo de dato para representar los planes disponibles
enum Plan
{
	// Plan por defecto (con anuncios)
	PLAN_FREEMIUM = 0,

	// Plan "pagado" (sin anuncios)
	PLAN_PREMIUM
};

// Estructura para representar un anuncio publicitario
struct Anuncio
{
	// Usuario anunciante
	 const char *anunciante;
	// URL del anuncio
	char *url;
};

// Estructura para representar un usuario
struct Usuario
{
	// Identificador único de usuario (utilizado en la base de datos)
	int id;
	
	// Usuario para iniciar sesión
	char *username;
	// Dirección de correo electrónico registrado
	char *email;
	// Contraseña para iniciar sesión
	char *password;

	// Nombre visible
	char *nickname;
	// Pais de origen del usuario
	char *pais;
	// Plan vigente de la cuenta
	Plan plan;

	// Perfil de artista
	Artista *artista;

	// Lista de amigos del usuario
	//Lista amigos;
	// Lista de playlists creadas por el usuario
	//Lista playlists;
	// Historial de reproducciones del usuario
	Historial historial;
};

typedef enum {
    TIPO_CANCION,
    TIPO_ANUNCIO
} ElementoColaTipo;

struct NodoAnuncio{
    // Anuncio publicitario
    Anuncio *anuncio;
    // Puntero al siguiente nodo
    NodoAnuncio *siguiente;
};
struct NodoCola {
    void* dato;                 // Puntero GENÉRICO. Apuntará a una CancionGobal o a un Anuncio.
    ElementoColaTipo tipo;      // La ETIQUETA que nos dice qué es 'dato'.
    struct NodoCola* siguiente; // Puntero al siguiente elemento en la cola.
} ;
struct NodoColadeReproduccion
{
	NodoCola* frente;  // Apunta al primer elemento (el que saldrá primero).
	NodoCola* final;   // Apunta al último elemento (para agregar nuevos fácilmente).
	int contadorCancionesFree; 
};

void insertarAnuncioCircular(NodoAnuncio **cabeza, Anuncio *anuncio) {
    NodoAnuncio *nuevo = (NodoAnuncio *)malloc(sizeof(NodoAnuncio));
    if (!nuevo) return;
    nuevo->anuncio = anuncio;
    nuevo->siguiente = NULL;

    if (*cabeza == NULL) {
        // Primer nodo: apunta a sí mismo
        nuevo->siguiente = nuevo;
        *cabeza = nuevo;
    } else {
        NodoAnuncio *temp = *cabeza;
        // Buscar el último nodo (el que apunta a cabeza)
        while (temp->siguiente != *cabeza) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
        nuevo->siguiente = *cabeza;
    }
}

void encolar(NodoColadeReproduccion* cola, void* dato, ElementoColaTipo tipo) {
    NodoCola* nuevoNodo = (NodoCola*)malloc(sizeof(NodoCola));
    if (!nuevoNodo) return; // Manejo de error en caso de falla de memoria

    nuevoNodo->dato = dato;
    nuevoNodo->tipo = tipo;
    nuevoNodo->siguiente = NULL;

    if (cola->frente == NULL) {
        // Cola vacía, el nuevo nodo es tanto el frente como el final
        cola->frente = nuevoNodo;
        cola->final = nuevoNodo;
    } else {
        // Encolar al final
        cola->final->siguiente= nuevoNodo;
        cola->final = nuevoNodo;
    }
}
void agregarCancionACola(NodoColadeReproduccion* cola, Usuario* usuario, Cancion* cancion, NodoAnuncio* cabezaAnuncio) {
    // 1. Siempre se agrega la canción a la cola.
    // Se usa la función 'encolar' interna, pasándole la canción y su TIPO.
    encolar(cola, cancion, TIPO_CANCION);

    // 2. Se aplica la lógica SOLO para usuarios free.
    if (usuario->plan == PLAN_FREEMIUM) {
        cola->contadorCancionesFree++; // 3. Se incrementa el contador.

        // 4. Se revisa si es momento de un anuncio.
        if (cola->contadorCancionesFree == 3) {
           
            // Se encola el anuncio, pasándole el anuncio y su TIPO.
            encolar(cola, cabezaAnuncio, TIPO_ANUNCIO);
            cabezaAnuncio=cabezaAnuncio->siguiente; // Avanzamos al siguiente anuncio.
            cola->contadorCancionesFree = 0; // 5. Se reinicia el contador.
        }
    }
}

NodoCola* desencolar(NodoColadeReproduccion* cola) {
    if (cola->frente == NULL) return NULL; // Cola vacía

    NodoCola* nodo = cola->frente;
    cola->frente = nodo->siguiente;

    // Si la cola queda vacía, actualiza también el final
    if (cola->frente == NULL) {
        cola->final = NULL;
    }

    nodo->siguiente = NULL; // Desvincula el nodo de la cola
    return nodo;
}


void reproducirCola(NodoColadeReproduccion* cola) {
    if (cola->frente==NULL){
    printf("La cola de reproducción está vacía.\n");
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
            printf("🎵 Reproduciendo: '%s' ", cancion->nombre);

        } else if (nodoActual->tipo == TIPO_ANUNCIO) {
            // 3. Es un anuncio: casteamos el puntero y lo procesamos.
            Anuncio* anuncio = (Anuncio*)nodoActual->dato;
            printf("📣 ANUNCIO: %s (%s)\n", anuncio->url, anuncio->anunciante);
        }

        // 4. Liberamos la memoria de la "caja" (el nodo), no del contenido.
        free(nodoActual);
    }
    printf("--- FIN DE LA REPRODUCCIÓN ---\n");
}
