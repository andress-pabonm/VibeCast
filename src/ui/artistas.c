#include <ui/interfaces.h>
#include <utils/utils.h>

// SUPONGO QUE TODOS LOS ARGUMENTOS QUE LLEGAN A LAS FUNCIONES, VIENE DE LA BASE DE DATOS O DE LA INTERFAZ GRAFICA

new_cmpfn(cmpArtistaConNombre)
{
    const Artista *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

new_cmpfn(cmpAlbumConNombre)
{
    const Album *a = val_1;
    const char *n = val_2;

    return strcmp(a->nombre, n);
}

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

    usuario->artista = artista; // Agregamos la estructura artista al usuario
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
    album->artista = usuario->artista; // AGREGAMOS ALBUM A ARTISTA

    // Insertamos en la lista, agregamos la estructura album al artista
    insertValueInLista(usuario->artista->albumes, album);

    return true;
}

bool agregarCancionAlbum(const char *nombreAlbum, const char *nombreCancion)
{
    // Buscamos el album
    //  if (!searchValueInLista(usuario->artista->albumes, nombreAlbum, cmpAlbumConNombre)) {
    //      printf("Error: El álbum '%s' no existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
    //      return false; // No se puede agregar una canción a un álbum que no existe
    //  }

    // Así se utiliza:

    // 1. Buscar el álbum:

    Album *album =
        searchValueInLista(
            usuario->artista->albumes,
            nombreAlbum,
            cmpAlbumConNombre);

    // 2. Verificar si encontró el álbum:

    if (!album)
    {
        printf("Error: El álbum '%s' no existe en el artista '%s'\n", nombreAlbum, usuario->artista->nombre);
        return false; // No se puede agregar una canción a un álbum que no existe
    }

    // Creamos nueva canción(No se si la logica seria aqui, por que faltaria mas campos, genero, fecha, duracion, etc)
    Cancion *cancion = newCancion(); // Todos sus datos inician en null
    if (!cancion)
        return false;

    cancion->nombre = asprintf(nombreCancion);

    // Esto no funciona, ya que album->nombre es (char *) y artista->albumes es (Lista)
    // cancion->album->nombre = usuario->artista->albumes;

    cancion->album = album;

    // Insertamos en la lista

    // Esto no es válido por las siguientes razones:
    // 1. cancion->album es (Album *), cuando la función espera (Lista)
    // 2. No se puede modificar la variable album desde la canción, ya que es (const)
    // insertValueInLista(cancion->album, cancion);

    insertValueInLista(album->canciones, cancion);

    return true;
}

static new_operfn(mostrar_canciones)
{
    // void *arg, int idx, void *val

    Cancion *cancion = val; // Canción actual (parecido a canciones[idx])

    printf("[%d]: %s\n", idx, cancion->nombre);

    return FOREACH_CONTINUE; // Para seguir con el bucle
}

// MOSTRAR ALBUMES DEL ARTISTA
void mostrarAlbum(Album *album)
{
    // int indice = 1; // No es necesario

    if (!album)
    {
        printf("Album no existente");
        return; // Supongo que esto debería ir, ¿no?
    }

    printf("Artista: %s\n", album->artista->nombre);            // Bien
    printf("Fecha de lanzamiento: %s\n", album->fechaCreacion); // Bien
    printf("Album: %s\n", album->nombre);                       // Bien
    printf("Canciones: \n");

    // while (album->canciones != NULL)
    // {
    //     printf("\t%d) %s\n", indice, album->canciones->start->val->nombre); // Referenciar bien el nombre de cada cancion por que la plena no se como xd
    // }
    // Ve a la linea 120
    // Entonces, lo que tienes que hacer es:
    forEachInLista(album->canciones, mostrar_canciones, NULL);

    // Nota: Si no hay canciones en la lista simplemente no se ejecutará mostrar_canciones()

    // No es necesario
    // if (album->canciones == NULL)
    // {
    //     printf("No hay canciones en este album\n");
    // }
}