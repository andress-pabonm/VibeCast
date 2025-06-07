#include <BaseDatos/crud_usuarios.h>
#include <SDL3/SDL.h>

#include <BaseDatos/crud_artistas.h> // Para la eliminación en cadena

/* Esta función creará un usuario en el heap */
Usuario *crear(Usuario, char *email, char *password, char *nombre, char *nickname, char *pais, Plan plan)
{
    Usuario usuario = {
        .email = NULL,
        .password = NULL,
        .nombre = NULL,
        .nickname = NULL,
        .pais = NULL,
        .plan = plan,

        .historial.reproducciones.cima = NULL,
        .historial.tiempoEscuchado = 0,
        .historial.cantidadAnuncios = 0,

        .playlists.inicio = NULL,
        .amigos.inicio = NULL,
        .perfilArtista = NULL};

    SDL_asprintf(&usuario.email, email);
    SDL_asprintf(&usuario.password, password);
    SDL_asprintf(&usuario.nombre, nombre);
    SDL_asprintf(&usuario.nickname, nickname);
    SDL_asprintf(&usuario.pais, pais);

    return alloc(Usuario, &usuario);
}

Usuario *leer(Usuario, ABB *usuarios, void *valor, cmpfn_t cmp)
{
    NodoABB *nodo = buscarNodo(ABB, usuarios, valor, cmp);
    return nodo ? nodo->valor : NULL;
}

void actualizar(Usuario, Usuario *usuario, char *email, char *password, char *nickname, char *nombre, char *pais)
{
    if (email)
    {
        SDL_free(usuario->email);
        SDL_asprintf(&usuario->email, email);
    }

    if (password)
    {
        SDL_free(usuario->password);
        SDL_asprintf(&usuario->password, password);
    }

    if (nombre)
    {
        SDL_free(usuario->nombre);
        SDL_asprintf(&usuario->nombre, nombre);
    }

    if (nickname)
    {
        SDL_free(usuario->nickname);
        SDL_asprintf(&usuario->nickname, nickname);
    }

    if (pais)
    {
        SDL_free(usuario->pais);
        SDL_asprintf(&usuario->pais, pais);
    }
}

void eliminar(Usuario, Usuario *usuario)
{
    /**
     * Esta función debería eliminar el espacio en memoria
     * que ocupa un determinado usuario.usuario.
     *
     * Aquí hay un problema, ya que no estoy seguro de los
     * criterios para eliminar un usuario. Asumo que un
     * usuario se puede eliminar Sólo si no tiene un perfil de
     * artista.
     *
     * ¿Por qué así?
     *
     * Porque si tuviera un perfil de artista y se quisiera eliminar,
     * habría que también eliminar sus álbumes y canciones de todos
     * los lugares donde aparecen.
     *
     * Nota:
     *
     * Ahora que lo pienso, esta función podría retornar true/false
     * si es posible eliminar un determinado usuario, entonces,
     * si lo mismo aplicara para las respectivas funciones de los
     * artistas, álbumes y canciones, podría encadenar las funciones.
     */

    //  En caso de que no se pueda eliminar el perfil de artista
    if (!eliminar(Artista, usuario->perfilArtista))
        return false; // No sería posible eliminar el usuario

    // Sino, empezaré a liberar la memoria
    SDL_free(usuario->email);
    SDL_free(usuario->password);
    SDL_free(usuario->nombre);
    SDL_free(usuario->nickname);
    SDL_free(usuario->pais);

    // Aquí libero la memoria de las estructuras de listas, pilas, etc.
    liberar(Pila, &usuario->historial.reproducciones, &liberarReproduccion);
    liberar(Lista, &usuario->playlists, &liberarPlaylist);
    liberar(Lista, &usuario->amigos, NULL);
}
