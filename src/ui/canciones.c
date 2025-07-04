#include <ui/interfaces.h>

//  Inserta una canci´ on en una lista.
void insertarCancion(Album *album, Cancion cancion ){
    Lista tmp = album->canciones;
    while (tmp->sig != NULL) {
        tmp = tmp->sig;
    }
    tmp->dato = cancion;
}

//  Crea y devuelve una nueva canci´ on.
bool crearCancion(Album *album, int id, const char *nombre, const char *genero, int duracion, const char *url)
{
    // Un álbum no puede tener canciones repetidas
    Lista *tmp = album->canciones;
    while (tmp->sig != NULL) {
        tmp = tmp->sig;
        if (strcmp(tmp->dato, nombre)) {
            printf("Una cancion con ese nombre ya existe en el album\n");
            return false;
        }
    }

    // Una canción se crea así:
    Cancion *cancion = newCancion(); // Inicia con sus valores en null
    if (!cancion)
        return false;
    cancion->id = id;
	cancion->album = album;		// Álbum al que pertenece
	strcpy(cancion->nombre, nombre);			// Nombre de la canción
	strcpy(cancion->genero, genero);	//char *genero;			// Género musical
	char *fechaPublicacion; // Fecha de publicación
    
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
    
	strcpy(cancion->fechaPublicacion, fecha_formateada);    
	cancion->duracion = duracion;
	strcpy(cancion->url, url);

    // Entonces aquí inicializas la canción


    // Luego tienes que enlazarla a su álbum
    //insertarCancion(album, cancion);
    insertValueInLista(album->canciones, cancion);
    

    
    //Inserta la cancion en su base de datos
    //Me falta ver como insertar el id del album
    char *datos = asprintf(
    stringify("%s", "%s", "%s", "%s", "%s"),
    album->id, cancion->nombre, cancion->genero, cancion->fechaPublicacion, cancion->duracion, cancion->url);

    nuevo_registro(
        "Canciones",
        "id_album, nombre, genero, fecha_publicacion, duracion, url",
        datos, NULL);

    return true;
}


//  Busca una canci´ on por nombre.
//Me falta saber cual es el puntero global a la lista de canciones
Cancion *buscarCancion(Lista canciones, int *id) {
    return searchValueInLista(canciones, &id, cmpCancionConId);
    
    /*
    Lista tmp = canciones;
    while (tmp->sig != NULL) {
        tmp = tmp->sig;
        if (strcmp(tmp->dato.nombre, nombre) == 0) {
            return cancion;
        }
    }
    printf ("La cancion no esta en la lista\n");
    return NULL;*/
}

//  Elimina una canci´on si no est´ a en una playlist.
void eliminarCancion(Lista **canciones, const char *nombre) {
    if (canciones == NULL || *canciones == NULL || nombre == NULL) {
        return;
    }

    Lista actual = *canciones;
    Lista anterior = NULL;

    // Buscar la canción en la lista
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) {
            // Eliminar de la lista principal
            if (anterior == NULL) {
                *canciones = actual->sig;  // Era el primer elemento
            } else {
                anterior->sig = actual->sig;
            }

            // Eliminar la canción de su álbum (si existe)
            if (actual->album != NULL && actual->album->canciones != NULL) {
                deleteValueInLista(actual->album->canciones, nombre, NULL);
            }

            free(actual);
            return;
        }
        anterior = actual;
        actual = actual->sig;
    }
}

//  Modifica datos de una canci´on.
void actualizarCancion(Cancion cancion, const char *nombre) {
    strcpy(cancion->nombre, nombre);

    //Actualizo en la base de datos
    char *datos = asprintf(
    stringify("%s"), nombre);

    nuevo_registro(
        "Canciones",
        "nombre",
        datos, NULL);
}