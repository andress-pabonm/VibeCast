#include <ui/interfaces.h>
#include <utils/utils.h>
#include<db/dbmgr.h>

new_cmpfn (cmpCancionConNombre) 
{
    const Cancion *c = val_1;
    const char *nombre = val_2;
    return strcmp(c->nombre, nombre);
}

new_cmpfn (cmpAlbumConNombre) 
{
    const Album *A = val_1;
    const char *nombre = val_2;
    return strcmp(A->nombre, nombre);
}

select_handler (obteneridCancion) 
{
    sscanf(argv[0], "%d", cast(int*, arg));
    return 0;
}

select_handler (obteneridCancion) 
{
    sscanf(argv[0], "%d", cast(int*, arg));
    return 0;
}

//  Crea y devuelve una nueva canci´ on.
bool crearCancion(Album *album,const char *nombre, const char *genero, int duracion, const char *url)
{
    // Un álbum no puede tener canciones repetidas
    Cancion *cancion = searchValueInLista(album->canciones, canciones, cmpCancionConNombre);
    if (cancion){
        printf("Una cancion con ese nombre ya existe en el album\n");
        return false;
    }

    // Una canción se crea así:
    cancion = newCancion(); // Inicia con sus valores en null
    if (!cancion)
        return false;

	cancion->album = album;
	cancion->nombre = asprintf(nombre);
	cancion->genero = asprintf(genero);
    
    //Se ingresa la hora

        time_t tiempo_actual;
        struct tm *info_tiempo;
        
        // Obtener el tiempo actual
        time(&tiempo_actual);
        info_tiempo = localtime(&tiempo_actual);
        
        // Reservar memoria para el string (20 caracteres son suficientes)
        char *fecha_formateada = (char*)malloc(20 * sizeof(char));
        
        if (fecha_formateada == NULL) {
            fprintf(stderr, "Error al asignar memoria\n");
            return NULL;
        }
        
        // Formatear la fecha: YYYY/MM/DD HH:MM:SS
        strftime(fecha_formateada, 20, "%Y/%m/%d %H:%M:%S", info_tiempo);
    
    cancion->fechaPublicacion = asprintf(fecha_formateada);
	cancion->duracion = duracion;
    cancion->url = asprintf(url);

    // Entonces aquí inicializas la canción


    // Luego tienes que enlazarla a su álbum
    insertValueInLista(album->canciones, cancion);

    //Inserta en la lista global de canciones
    insertValueInLista(canciones, cancion);
    
    //Inserta la cancion en su base de datos
    char *datos = asprintf(
    stringify("%s", "%s", "%s", "%s", "%s"),
    album->id, cancion->nombre, cancion->genero, cancion->fechaPublicacion, cancion->duracion, cancion->url);

    nuevo_registro(
        "Canciones",
        "id_album, nombre, genero, fecha_publicacion, duracion, url",
        datos, NULL);
    
    //Inserta el id
    obtener_registros(
        "Canciones ORDER BY id DESC LIMIT 1",
        "id",
        NULL,
        obteneridCancion,
        &cancion->id,
        NULL
    );

    return true;
}

//  Busca una canci´ on por nombre.
//Me falta saber cual es el puntero global a la lista de canciones
Cancion *buscarCancion(Lista canciones, int *id) {
    return searchValueInLista(canciones, &id, cmpCancionConId);
}

//  Elimina una canci´on si no est´ a en una playlist.
void eliminarCancion(int id) {
    Cancion *cancion = searchValueInLista(canciones, &id, cmpCancionConId);
    
    if (cancion == NULL){
        printf ("La cancion con el id %d no existe \n", id);
        return;
    }

    Album *album = searchValueInLista (usuario->artista->albumes, &(cancion->album->nombre), cmpAlbumConNombre);
    deleteValueInLista(album->canciones, &id, cmpCancionConId);
    deleteValueInLista(canciones, &id, cmpCancionConId);
    free(cancion->fechaPublicacion);
    free(cancion->genero);
    free(cancion->nombre);
    free(cancion->url);  

    char *condicion = asprintf("ID = %d", id);
    eliminar_registros(
        "Canciones",
        "condicion",
        NULL
    );

    free(cancion);
}

//  Modifica datos de una canci´on.
void actualizarCancion(Cancion *cancion, const char *nombre) {
    cancion->nombre = asprintf(nombre);

    //Actualizo en la base de datos
    char *datos = asprintf(
    stringify("%s"), nombre);

    nuevo_registro(
        "Canciones",
        "nombre",
        datos, NULL);
}
