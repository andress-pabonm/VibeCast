#include <ui/interfaces.h>

// SUPONGO QUE TODOS LOS ARGUMENTOS QUE LLEGAN A LAS FUNCIONES, VIENE DE LA BASE DE DATOS O DE LA INTERFAZ GRAFICA

static new_cmpfn(cmpArtistaConNombre)
{ 
    const Artista *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

static new_cmpfn(cmpAlbumConNombre)
{
    const Album *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

static new_operfn(mostrar_canciones)
{
    Cancion *cancion = val; // Canción actual

    printf("[%d]: %s\n", idx, cancion->nombre); // Muestra cancion actual

    return FOREACH_CONTINUE; // Para seguir con el bucle
}

/* ================ Funcionalidad principal ================================ */

// Sugerencia: Utilizar la macro interfaz()
interfaz(crearArtista)
{
    // Aquí tienes acceso a las siguientes variables
    // void *arg: Algún parámetro extra de ser necesario
    // int argc: Cantidad de argumentos en argv
    // const char *argv[]: Un arreglo de strings con la información que se le pasa a la función
    // char **msg: Variable donde se puede devolver algún mensaje de ser necesario

    // Ejemplo de uso:

    const char *nombreArtista = argv[0];

    Artista *artista = searchValueInABB(artistas, nombreArtista, cmpArtistaConNombre);

    if (!artista)
    {
        send_message("El nombre de artista ya existe.");
        return false;
    }

    artista = newArtista(); // Inicia con sus atributos en null

    artista->usuario = usuario; // Enlazar perfil de usuario
    artista->nombre = asprintf(nombreArtista);

    usuario->artista = artista; // Enlazar perfil de artista

    // Ejemplo con arg:

    // Usuario *u = arg; // Suponiendo que arg es un puntero a un usuario
    // artista->usuario = u;
    // u->artista = artista;

    return true;
}

message_handler(crear_artista)
{
    init_data_json();

    const char *nombreArtista =
        get_string(get_array_idx(data, 0));

    const char *argv[] = {
        nombreArtista,
    };
    int argc = sizeof(argv) / sizeof(*argv);

    char **msg = arg;
    bool success = VibeCast_crearArtista(usuario, argc, argv, msg);

    VibeCast_SendText(
        id,
        HTTP_OK,
        *msg,
        success ? "Artista creado" : "Fallo al crear artista",
        STATE_BOOL(success));

    puts(*msg); // Imprimir el mensaje en consola

    freem(*msg); // Liberar la memoria
    *msg = NULL; // Reiniciar la variable
}
// Fin de sugerencia

bool crearNombreArtista(const char *nombreArtista)
{
    // Buscamos si el nombre del artista ya existe
    if (searchValueInABB(artistas, nombreArtista, cmpArtistaConNombre))
    {
        printf("Error: El artista '%s' ya existe\n", nombreArtista);

        return false; // No se puede crear un artista con un nombre ya existente
    }

    // CREAMOS ARTISTA
    Artista *artista = newArtista(); // Todos sus datos inician en null
    if (!artista)
        return false;

    usuario->artista = artista; // Enlazamos el perfil de artista a su usuario
    artista->nombre = asprintf(nombreArtista);
    artista->usuario = usuario;

    // Insertamos en el arbol
    insertValueInABB(artistas, artista);

    return true;
}

bool crearAlbum(const char *nombreAlbum)
{
    // Buscamos si el album ya existe en los albumes del artista
    if (searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre))
    {
        printf("Error: El álbum '%s' ya existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return false; // No se puede crear un álbum con un nombre ya existente
    }

    // CREAMOS NUEVO ALBUM
    Album *album = newAlbum(); // Todos sus datos inician en null
    if (!album)
        return false;

    album->nombre = asprintf(nombreAlbum);
    album->artista = usuario->artista; // ENLAZAMOS ALBUM A ARTISTA

    // Insertamos en la lista, agregamos la estructura album al artista
    insertValueInLista(usuario->artista->albumes, album);

    return true;
}

bool agregarCancionAlbum(const char *nombreAlbum, const char *nombreCancion)
{
    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre); // Buscamos el álbum y lo asignamos a la variable album

    if (!album)
    {
        printf("Error: El álbum '%s' no existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return false; // No se puede agregar una canción a un álbum que no existe
    }

    // Creamos nueva canción
    // (No se si la logica seria aqui, por que faltaria mas campos, genero, fecha, duracion, etc)
    Cancion *cancion = newCancion(); // Todos sus datos inician en null
    if (!cancion)
        return false;

    cancion->nombre = asprintf(nombreCancion);
    cancion->album = album; // AGREGAMOS CANCION A ALBUM

    // Insertamos en la lista
    insertValueInLista(album->canciones, cancion);

    return true;
}

// MOSTRAR ALBUMES DEL ARTISTA
void mostrarAlbum(const char *nombreAlbum)
{
    Album *album = searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre); // Buscamos el álbum y lo asignamos a la variable album

    if (!album)
    {
        printf("Album no existente");
        return; // Retornar para evitar errores con las siguientes instrucciones
    }

    printf("Artista: %s\n", album->artista->nombre);
    printf("Fecha de lanzamiento: %s\n", album->fechaCreacion);
    printf("Album: %s\n", album->nombre);
    printf("Canciones: \n");

    // Funcion que actua como bucle para mostrar todas las canciones que haya en el album
    // Si no hay canciones en la lista simplemente no se ejecutará mostrar_canciones()
    forEachInLista(album->canciones, mostrar_canciones, NULL);
}
