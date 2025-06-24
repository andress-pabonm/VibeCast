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
	// Puntero a su perfil de usuario
	Usuario *usuario;
	// Nombre del artista
	char *nombre;
	// Lista de los álbumes del artista
	Lista albumes;
};

// Estructura para representar un álbum de canciones
struct Album
{
	// Identificador único de álbum (utilizado en la base de datos)
	int id;
	// Artista al que pertenece el álbum
	Artista *artista;
	// Nombre del álbum
	char *nombre;
	// Fecha en la que se creó el álbum
	char *fechaCreacion;
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
	char *nombre;
	// Género musical al que pertenece la canción
	char *genero;
	// Fecha en la que se publicó la canción en la app
	char *fechaPublicacion;

	// Duración de la canción
	int duracion;
	// URL donde se encuentra la canción
	char *url;

	// Cantidad de veces que se ha añadido a una playlist
	int popularidad;
	// Cantidad total de reproducciones de la canción
	int reproducciones;
};

// Estructura para representar una playlist
struct Playlist
{
	// Identificador único de playlist (utilizado en la base de datos)
	int id;
	// Nombre de la playlist
	char *nombre;
	// Lista con las canciones agregadas a la playlist
	Lista canciones;
};

// Estructura para representar el historial de un usuario
struct Historial
{
	// Pila con las reproducciones
	Pila reproducciones;
	// Tiempo total escuchado/reproducido
	int tiempoEscuchado;
	// Cantidad de anuncios "vistos" por el usuario (PLAN_FREEMIUM)
	int cantidadAnuncios;
};

// Estructura para representar una reproducción de una canción
struct Reproduccion
{
	// Canción reproducida
	Cancion *cancion;
	// Fecha de la reproducción
	char *fechaEscuchado;
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
	// Usuario anunciante
	Usuario *anunciante;
	// URL del anuncio
	char *url;
};

// Estructura para representar un usuario
struct Usuario
{
	// Identificador único de usuario (utilizado en la base de datos)
	int id;
	
	// Usuario para iniciar sesión
	char *username;
	// Dirección de correo electrónico registrado
	char *email;
	// Contraseña para iniciar sesión
	char *password;

	// Nombre visible
	char *nickname;
	// Pais de origen del usuario
	char *pais;
	// Plan vigente de la cuenta
	Plan plan;

	// Perfil de artista
	Artista *artista;

	// Lista de amigos del usuario
	Lista amigos;
	// Lista de playlists creadas por el usuario
	Lista playlists;
	// Historial de reproducciones del usuario
	Historial historial;
};

#endif // ESTRUCTURAS_DB_H