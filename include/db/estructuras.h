#ifndef ESTRUCTURAS_DB_H
#define ESTRUCTURAS_DB_H

#include <utils/nodos.h> // Para Lista, Pila y Cola

// Estructuras para la base de datos de música tipo Spotify

typedef struct Artista Artista;
typedef struct Album Album;
typedef struct Cancion Cancion;

typedef struct Playlist Playlist;

typedef struct Historial Historial;
typedef struct Reproduccion Reproduccion;

typedef enum Plan Plan;
typedef struct Anuncio Anuncio;

typedef struct Usuario Usuario;

struct Artista
{
	// Nombre del artista
	char *nombreArtista;
	// Lista de los álbumes del artista
	Lista albumes;
};

// Estructura para representar un álbum de canciones
struct Album
{
	// Artista al que pertenece el álbum
	Artista *artista;
	// Nombre del álbum
	char *nombreAlbum;
	// Fecha en la que se creó el álbum
	char fechaCreacion[11];
	// Lista de canciones del álbum
	Lista canciones;
};

// Estructura para representar una canción
struct Cancion
{
	// Identificador de la canción
	int id;
	
	// Álbum al que pertenece la canción
	Album *album;
	// Título de la canción
	char *nombreCancion;
	// Género musical al que pertenece la canción
	char *genero;
	// Fecha en la que se publicó la canción en la app
	char fechaLanzamiento[11];

	// Cantidad de veces que se ha añadido a una playlist
	int popularidad;
	// Cantidad total de reproducciones de la canción
	int reproducciones;

	// Duración de la canción
	float duracion;
	// URL donde se encuentra la canción
	char *url;
};

// Estructura para representar una playlist
struct Playlist
{
	// Nombre de la playlist
	char *nombrePlaylist;
	// Fecha en la que se creó la playlist
	// char fechaCreacion[11];
	// Lista con las canciones agregadas a la playlist
	Lista canciones;
};

// Estructura para representar el historial de un usuario
struct Historial
{
	// Pila con las reproducciones
	Pila reproducciones;
	// Tiempo total escuchado/reproducido
	float tiempoEscuchado;
	// Cantidad de anuncios "vistos" por el usuario (PLAN_FREEMIUM)
	int cantidadAnuncios;
};

// Estructura para representar una reproducción de una canción
struct Reproduccion
{
	// Canción reproducida
	Cancion *cancion;
	// Fecha de la reproducción
	char fechaEscuchado[11];
};

// Tipo de dato para representar los planes disponibles
enum Plan
{
	// Plan por defecto (con anuncios)
	PLAN_FREEMIUM = 0,

	// Plan "pagado" (sin anuncios)
	PLAN_PREMIUM
};

// Estructura para representar un anuncio publicitario
struct Anuncio
{
	// Nombre del anunciante
	Usuario *anunciante;
	// URL del anuncio
	char *url;
};

// Estructura para representar un usuario
struct Usuario
{
	// Dirección de correo electrónico registrado
	char *email;
	// Contraseña para iniciar sesión
	char *password;

	// Identificador único de usuario
	char *username;
	// Nombre visible
	char *nickname;

	// Pais de origen del usuario
	char *pais;
	// Plan vigente de la cuenta
	Plan plan;

	// Lista de playlists creadas por el usuario
	Lista playlists;
	// Historial de reproducciones del usuario
	Historial historial;

	// Lista de amigos del usuario
	Lista amigos;

	// Perfil de artista
	Artista *artista;
};

#endif // ESTRUCTURAS_DB_H