#include <ui/interfaces.h>

//  Crea y devuelve una nueva canci´ on.
bool crearCancion(Album *album, const char *nombre)
{
    // Un álbum no puede tener canciones repetidas

    // Una canción se crea así:
    Cancion *cancion = newCancion(); // Inicia con sus valores en null
    if (!cancion)
        return false;

    // Entonces aquí inicializas la canción
    // Luego tienes que enlazarla a su álbum

    return true;
}

//  Inserta una canci´ on en una lista.
// void insertarCancion(...);

//  Busca una canci´ on por nombre.
// Cancion *buscarCancion(...);

//  Elimina una canci´on si no est´ a en una playlist.
// void eliminarCancion(...);

//  Modifica datos de una canci´on.
// void actualizarCancion(...);

//  Verifica si la canci´on est´ a en alguna playlist
// bool cancionPerteneceAPlaylist(...);
