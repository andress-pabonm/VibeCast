#ifndef VIBECAST_DB_ESTRUCTURAS_H
#define VIBECAST_DB_ESTRUCTURAS_H 1

#include <utils/nodos.h> // Para Lista, Pila y Cola

// ==========================================
// Tipos hacia adelante (punteros a structs)
// ==========================================

typedef struct Artista Artista;
typedef struct Album Album;
typedef struct Cancion Cancion;

typedef struct Playlist Playlist;

typedef struct Historial Historial;
typedef struct Reproduccion Reproduccion;

typedef enum Plan Plan;
typedef struct Anuncio Anuncio;

typedef struct Usuario Usuario;

// ==========================================
// Enumeraciones
// ==========================================

enum Plan
{
	PLAN_FREEMIUM = 0, // Plan con anuncios
	PLAN_PREMIUM	   // Plan sin anuncios
};

// ==========================================
// Estructuras
// ==========================================

// ---------- Reproducción e historial ----------

struct Reproduccion
{
	const Cancion *cancion; // Canción reproducida
	char *fechaEscuchado;	// Fecha de reproducción
};

struct Historial
{
	Pila reproducciones;  // Pila de reproducciones
	int tiempoEscuchado;  // Tiempo total escuchado (en segundos)
	int cantidadAnuncios; // Anuncios vistos (solo en FREEMIUM)
};

// ---------- Canciones y álbumes ----------

struct Cancion
{
	int id;					// ID único
	const Album *album;		// Álbum al que pertenece
	char *nombre;			// Nombre de la canción
	char *genero;			// Género musical
	char *fechaPublicacion; // Fecha de publicación
	int duracion;			// Duración en segundos
	char *url;				// URL de reproducción
	int popularidad;		// Veces añadida a playlists
	int reproducciones;		// Reproducciones totales
};

struct Album
{
	int id;					// ID único
	const Artista *artista; // Artista propietario
	char *nombre;			// Nombre del álbum
	char *fechaCreacion;	// Fecha de creación
	Lista canciones;		// Lista de canciones (Lista de Cancion)
};

// ---------- Artista ----------

struct Artista
{
	const Usuario *usuario; // Perfil de usuario asociado
	char *nombre;			// Nombre artístico
	Lista albumes;			// Lista de álbumes (Lista de Album)
};

// ---------- Playlist ----------

struct Playlist
{
	int id;			 // ID único
	char *nombre;	 // Nombre de la playlist
	Lista canciones; // Lista de canciones (Lista de Cancion)
};

// ---------- Anuncios ----------

struct Anuncio
{
	int id;					   // Identificador único de anuncio
	const Usuario *anunciante; // Usuario que publica el anuncio
	char *url;				   // URL del anuncio
};

// ---------- Usuario ----------

struct Usuario
{
	int id;			// ID único
	char *username; // Nombre de usuario
	char *email;	// Correo electrónico
	char *password; // Contraseña

	char *nickname; // Nombre visible
	char *pais;		// País de origen
	Plan plan;		// Plan actual

	Artista *artista; // Perfil de artista (opcional)

	Lista amigos;		 // Lista de amigos (Lista de Usuario)
	Lista playlists;	 // Playlists creadas (Lista de Playlist)
	Historial historial; // Historial de reproducción

	time_t caducidadPremium; // Fecha de caducidad del plan premium (0 si no es premium)
};

// ==========================================
// Constructores y destructores de estructuras
// ==========================================

Usuario *newUsuario(void);
void destroyUsuario(Usuario *usuario);
new_operfn(destroyUsuarios);

Artista *newArtista(void);
void destroyArtista(Artista *artista);
new_operfn(destroyArtistas);

Album *newAlbum(void);
void destroyAlbum(Album *album);
new_operfn(destroyAlbumes);

Cancion *newCancion(void);
void destroyCancion(Cancion *cancion);
new_operfn(destroyCanciones);

Playlist *newPlaylist(void);
void destroyPlaylist(Playlist *playlist);
new_operfn(destroyPlaylists);

Reproduccion *newReproduccion(void);
void destroyReproduccion(Reproduccion *reproduccion);
new_operfn(destroyReproducciones);

Anuncio *newAnuncio(void);
void destroyAnuncio(Anuncio *anuncio);
new_operfn(destroyAnuncios);

#endif // VIBECAST_DB_ESTRUCTURAS_H
