/* Aquí se cargan los datos desde la base datos y se los organiza en las diferentes estructuras. */

#include <db/estructuras.h>

/**
 * 1. Cargar toda la información a la vez (usuarios, artistas, álbumes y canciones)
 * 2. Crear los usuarios y añadirlos a un árbol binario
 * 3. Por cada usuario: Crear los perfiles de artista, enlazarlos y añadirlos a un árbol
 * 3. Por cada artista: Crear los álbumes, enlazarlos y añadirlos a una lista
 * 4. Por cada álbum: Crear las canciones, enlazarlas y añadirlas en una lista
 *
 * Nota: Como las canciones no tienen una referencia a qué álbum pertenecen,
 * sería de realizar primero el paso 4 y luego el 3, creo.
 */

static ABB usuarios = {NULL};
static ABB artistas = {NULL};
static Lista albumes = {NULL};
static Lista canciones = {NULL};

void VibeCast_CargarDatos()
{
    void *datos_usuarios = NULL;
    size_t cantidad_usuarios = VibeCast_CargarUsuarios(&datos_usuarios);

    if (!cantidad_usuarios) // Si no hay usuarios registrados
        return;             // Entonces no habrá registros para artistas, álbumes y canciones
    // Es decir, solo habrá estos datos si hay usuarios.

    void *datos_artistas = NULL;
    size_t cantidad_artistas = VibeCast_CargarArtistas(&datos_artistas);

    void *datos_albumes = NULL;
    size_t cantidad_albumes = VibeCast_CargarAlbumes(&datos_albumes);

    void *datos_canciones = NULL;
    size_t cantidad_canciones = VibeCast_CargarCanciones(&datos_canciones);

    /* Crear las estructuras base */
    /* Aquí iría el código para que iterativamente se vayan creando las estructuras */

    // Para usuarios
    for (size_t i = 0; i < cantidad_usuarios; i++)
    {
        // char *nickname = datos_usuarios[i].nickname;
        // char *email = datos_usuarios[i].email;
        // char *password = datos_usuarios[i].password;

        // Usuario *usuario = crear(Usuario, nickname, email, password);
        // NodoABB *nodo = nuevoNodo(ABB, usuario);
        // insertarNodo(ABB, &usuarios, nodo, &cmpUsuario);
    }

    // Para artistas
    for (size_t i = 0; i < cantidad_artistas; i++)
    {
        // char *nombre = datos_artistas[i].nombre;

        // Arista *artista = crear(Artista, nombre);
        // NodoABB *nodo = nuevoNodo(ABB, artista);
        // insertarNodo(ABB, &artistas, nodo, &cmpArtista);
    }

    // Para albumes
    for (size_t i = 0; i < cantidad_albumes; i++)
    {
        // char *nombre = datos_albumes[i].nombre;

        // Album *album = crear(Album, nombre);
        // NodoLista *nodo = nuevoNodo(Lista, album);
        // insertarNodo(Lista, &albumes, nodo, NULL);
    }

    // Para canciones
    for (size_t i = 0; i < cantidad_canciones; i++)
    {
        // char *nombre = datos_canciones[i].nombre;
        // char *archivo = datos_canciones[i].archivo;

        // Cancion *cancion = crear(Cancion, nombre, archivo);
        // NodoLista *nodo = nuevoNodo(Lista, cancion);
        // insertarNodo(Lista, &canciones, nodo, NULL);
    }

    /* Una vez creadas las estructuras,  */
    /* ... */
}

Usuario *VibeCast_BuscarUsuario()
{
    buscarNodo(ABB, &usuarios, NULL, NULL);
    return NULL;
}