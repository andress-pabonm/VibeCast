#include <db/estructuras.h>
#include <utils/memmgr.h>
#include <stddef.h>

/* ================================================================ */
// FUNCIONES DE USUARIO
/* ================================================================ */

Usuario *newUsuario(void)
{
    Usuario *u = alloc(Usuario, NULL);
    if (!u)
        return NULL;

    u->id = 0;
    u->username = NULL;
    u->email = NULL;
    u->password = NULL;
    u->nickname = NULL;
    u->pais = NULL;
    u->plan = PLAN_FREEMIUM;
    u->artista = NULL;

    u->amigos = newLista(NULL);
    if (!u->amigos)
    {
        freem(u);
        return NULL;
    }

    u->playlists = newLista(NULL);
    if (!u->playlists)
    {
        destroyLista(u->amigos, NULL, NULL);
        freem(u);
        return NULL;
    }

    u->historial.reproducciones = newPila();
    if (!u->historial.reproducciones)
    {
        destroyLista(u->amigos, NULL, NULL);
        destroyLista(u->playlists, NULL, NULL);
        freem(u);
        return NULL;
    }

    u->historial.tiempoEscuchado = 0;
    u->historial.cantidadAnuncios = 0;

    return u;
}

void destroyUsuario(Usuario *usuario)
{
    if (!usuario)
        return;

    freem(usuario->username);
    freem(usuario->email);
    freem(usuario->password);
    freem(usuario->nickname);
    freem(usuario->pais);

    destroyLista(usuario->amigos, NULL, NULL);
    destroyLista(usuario->playlists, destroyPlaylists, NULL);
    destroyPila(usuario->historial.reproducciones, destroyReproducciones, NULL);
    destroyArtista(usuario->artista);

    freem(usuario);
}

new_operfn(destroyUsuarios)
{
    destroyUsuario(val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// FUNCIONES DE ARTISTA
/* ================================================================ */

Artista *newArtista(void)
{
    Artista *a = alloc(Artista, NULL);
    if (!a)
        return NULL;

    a->usuario = NULL;
    a->nombre = NULL;
    a->albumes = newLista(NULL);
    if (!a->albumes)
    {
        freem(a);
        return NULL;
    }

    return a;
}

void destroyArtista(Artista *artista)
{
    if (!artista)
        return;

    freem(artista->nombre);
    destroyLista(artista->albumes, destroyAlbumes, NULL);

    freem(artista);
}

new_operfn(destroyArtistas)
{
    destroyArtista(val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// FUNCIONES DE ÁLBUM
/* ================================================================ */

Album *newAlbum(void)
{
    Album *a = alloc(Album, NULL);
    if (!a)
        return NULL;

    a->id = 0;
    a->artista = NULL;
    a->nombre = NULL;
    a->fechaCreacion = NULL;
    a->canciones = newLista(NULL);
    if (!a->canciones)
    {
        freem(a);
        return NULL;
    }

    return a;
}

void destroyAlbum(Album *album)
{
    if (!album)
        return;

    freem(album->nombre);
    freem(album->fechaCreacion);
    destroyLista(album->canciones, destroyCanciones, NULL);

    freem(album);
}

new_operfn(destroyAlbumes)
{
    destroyAlbum(val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// FUNCIONES DE CANCIÓN
/* ================================================================ */

Cancion *newCancion(void)
{
    Cancion *c = alloc(Cancion, NULL);
    if (!c)
        return NULL;

    c->id = 0;
    c->album = NULL;
    c->nombre = NULL;
    c->genero = NULL;
    c->fechaPublicacion = NULL;
    c->duracion = 0;
    c->url = NULL;
    c->popularidad = 0;
    c->reproducciones = 0;

    return c;
}

void destroyCancion(Cancion *cancion)
{
    if (!cancion)
        return;

    freem(cancion->nombre);
    freem(cancion->fechaPublicacion);
    freem(cancion->genero);
    freem(cancion->url);

    freem(cancion);
}

new_operfn(destroyCanciones)
{
    destroyCancion(val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// FUNCIONES DE PLAYLIST
/* ================================================================ */

Playlist *newPlaylist(void)
{
    Playlist *p = alloc(Playlist, NULL);
    if (!p)
        return NULL;

    p->id = 0;
    p->nombre = NULL;
    p->canciones = newLista(NULL);
    if (!p->canciones)
    {
        freem(p);
        return NULL;
    }

    return p;
}

void destroyPlaylist(Playlist *playlist)
{
    if (!playlist)
        return;

    freem(playlist->nombre);
    destroyLista(playlist->canciones, NULL, NULL);

    freem(playlist);
}

new_operfn(destroyPlaylists)
{
    destroyPlaylist(val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// FUNCIONES DE REPRODUCCIÓN
/* ================================================================ */

Reproduccion *newReproduccion(void)
{
    Reproduccion *r = alloc(Reproduccion, NULL);
    if (!r)
        return NULL;

    r->cancion = NULL;
    r->fechaEscuchado = NULL;

    return r;
}

void destroyReproduccion(Reproduccion *reproduccion)
{
    if (!reproduccion)
        return;

    freem(reproduccion->fechaEscuchado);

    freem(reproduccion);
}

new_operfn(destroyReproducciones)
{
    destroyReproduccion(val);
    return FOREACH_CONTINUE;
}

/* ================================================================ */
// FUNCIONES DE ANUNCIO
/* ================================================================ */

Anuncio *newAnuncio(void)
{
    Anuncio *a = alloc(Anuncio, NULL);
    if (!a)
        return NULL;

    a->anunciante = NULL;
    a->url = NULL;

    return a;
}

void destroyAnuncio(Anuncio *anuncio)
{
    if (!anuncio)
        return;

    freem(anuncio->url);

    freem(anuncio);
}

new_operfn(destroyAnuncios)
{
    destroyAnuncio(val);
    return FOREACH_CONTINUE;
}
