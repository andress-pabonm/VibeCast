#include <ui/interfaces.h>
#include <db/dbmgr.h>

new_cmpfn(cmpCancionConNombre)
{
    const Cancion *c = val_1;         // Puntero a Cancion
    const char *nombre = val_2;       // Puntero a nombre (string) a comparar
    return strcmp(c->nombre, nombre); // Compara el nombre de la canción con el nombre dado
}

new_cmpfn(cmpAlbumConNombre)
{
    const Album *A = val_1;           // Puntero a Album
    const char *nombre = val_2;       // Puntero a nombre (string) a comparar
    return strcmp(A->nombre, nombre); // Compara el nombre del álbum con el nombre dado
}

select_handler(obteneridCancion)
{
    sscanf(argv[0], "%d", cast(int *, arg)); // Asigna el ID de la canción al puntero proporcionado
    return 0;
}

Cancion *crearCancion(Album *album, const char *nombre, const char *genero, int duracion, const char *url)
{
    datetime_buf_t fechaActual;
    getDateTime(fechaActual, now());

    // Verificar si la canción ya existe en el álbum
    Cancion *cancion = searchValueInLista(album->canciones, nombre, cmpCancionConNombre);
    if (cancion)
    {
        printf("Una cancion con ese nombre ya existe en el album\n");
        return NULL;
    }

    // Crear nueva estructura de canción
    cancion = newCancion();

    // Configurar propiedades de la canción
    cancion->album = album;
    cancion->nombre = asprintf(nombre);
    cancion->genero = asprintf(genero);
    cancion->fechaPublicacion = asprintf(fechaActual);
    cancion->duracion = duracion;
    cancion->url = asprintf(url);

    // Insertar en las estructuras de datos
    insertValueInLista(canciones, cancion);
    insertValueInLista(album->canciones, cancion);

    // Registrar en base de datos
    char *datos = asprintf(
        stringify("%s", "%s", "%s", "%s", "%s"),
        album->id, cancion->nombre, cancion->genero,
        cancion->fechaPublicacion, cancion->duracion, cancion->url);

    nuevo_registro(
        "Canciones",
        "id_album, nombre, genero, fecha_publicacion, duracion, url",
        datos, NULL);

    // Obtener ID asignado por la base de datos
    obtener_registros(
        "Canciones ORDER BY id DESC LIMIT 1",
        "id",
        NULL,
        obteneridCancion,
        &cancion->id,
        NULL);

    return cancion;
}

void eliminarCancion(int id)
{
    // Buscar la canción en la lista global
    Cancion *cancion = searchValueInLista(canciones, &id, cmpCancionConId);

    if (cancion == NULL)
    {
        printf("La canción con el id %d no existe\n", id);
        return;
    }

    // Verifica si hay al menos una playlist que contiene la canción
    // if ()
    // {
    //     // Logica para encontrar al menos una playlist que contenga la canción
    // }

    // Buscar el álbum al que pertenece la canción
    Album *album = searchValueInLista(usuario->artista->albumes, &(cancion->album->nombre), cmpAlbumConNombre);
    if (!album)
    {
        printf("No se encontró el álbum asociado a la canción\n");
        return;
    }

    // Eliminar de las estructuras de datos
    deleteValueInLista(album->canciones, &id, cmpCancionConId);
    deleteValueInLista(canciones, &id, cmpCancionConId);

    // Liberar memoria de los strings
    free(cancion->fechaPublicacion);
    free(cancion->genero);
    free(cancion->nombre);
    free(cancion->url);

    // Eliminar de la base de datos APLCAR LA LOGICA NECESARIA
    // char *condicion = asprintf("ID = %d", id);
    // eliminar_registros("Canciones", condicion, NULL);
    // free(condicion);

    // Liberar la estructura de la canción
    free(cancion);

    printf("Canción eliminada exitosamente\n");
}

void actualizarCancion(Cancion *cancion, const char *nombre)
{
    // Liberar nombre anterior
    free(cancion->nombre);

    // Asignar nuevo nombre
    cancion->nombre = asprintf(nombre);

    // Actualizar en base de datos APLICAR LOGICA NECESARIA
    // char *datos = asprintf(stringify("%s"), nombre);
    // nuevo_registro("Canciones", "nombre", datos, NULL);
}

// APLCAR LOGICA NECESARIA
//  void crearAlbumDB(Artista *artista, const char *nombre)
//  {
//      const char *fecha_creacion = obtenerFecha();
//      char *datos = asprintf(
//          stringify("%s"),
//          artista->usuario->id, artista->nombre, fecha_creacion);

//     nuevo_registro(
//         "Albumes",
//         "id_artista, nombre, fecha_creacion",
//         datos, NULL);
// }