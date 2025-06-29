#include <db/estructuras.h>
#include <utils/memmgr.h>
#include <stddef.h>

// ==========================
// Funciones para crear structs
// ==========================

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

Reproduccion *newReproduccion(void)
{
    Reproduccion *r = alloc(Reproduccion, NULL);
    if (!r)
        return NULL;

    r->cancion = NULL;
    r->fechaEscuchado = NULL;

    return r;
}

Anuncio *newAnuncio(void)
{
    Anuncio *a = alloc(Anuncio, NULL);
    if (!a)
        return NULL;

    a->anunciante = NULL;
    a->url = NULL;

    return a;
}
