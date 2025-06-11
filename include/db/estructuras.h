#ifndef ESTRUCTURAS_DB_H
#define ESTRUCTURAS_DB_H

#include <utils/nodos.h> // Para Lista, Pila y Cola

// Estructuras para la base de datos de música tipo Spotify

typedef struct Usuario Usuario;
typedef enum Plan Plan;
typedef struct Historial Historial;
typedef struct Reproduccion Reproduccion;
typedef struct Playlist Playlist;
typedef struct Cancion Cancion;
typedef struct Artista Artista;
typedef struct Album Album;
typedef struct ColaReproduccion ColaReproduccion;
typedef struct Anuncio Anuncio;

// Tipos de plan disponibles
enum Plan {
	PLAN_FREEMIUM,  // Con anuncios
	PLAN_PREMIUM    // Sin anuncios
};

// Historial de un usuario (canciones escuchadas, tiempo, anuncios)
struct Historial {
	Pila reproducciones;   // Pila de Reproduccion*
	int tiempoEscuchado;   // Tiempo total en segundos
	int cantidadAnuncios;  // Cantidad de anuncios escuchados
};

// Representa una reproducción individual
struct Reproduccion {
	Cancion *cancion;  // Canción reproducida
	char *fecha;       // Fecha de reproducción (formato YYYY-MM-DD)
};

// Representa un usuario de la plataforma
struct Usuario {
	char *email;          // Email único del usuario
	char *password;       // Contraseña (hash recomendada en producción)
	char *nombre;         // Nombre real
	char *nickname;       // Alias público
	char *pais;           // País de origen
	Plan plan;            // Plan actual del usuario
	Historial historial;  // Historial de reproducciones
	Lista playlists;      // Lista de Playlist*
	Lista amigos;         // Lista de Usuario*
	Artista *perfilArtista; // Si tiene perfil como artista
};

// Lista de canciones personalizadas por un usuario
struct Playlist {
	char *nombre;      // Nombre de la playlist
	Lista canciones;   // Lista de Cancion*
};

// Representa a un artista musical
struct Artista {
	char *nombre;      // Nombre artístico
	Lista albumes;     // Lista de Album*
};

// Representa un álbum musical
struct Album {
	char *nombre;      // Nombre del álbum
	char *fecha;       // Fecha de lanzamiento
	Lista canciones;   // Lista de Cancion*
};

// Representa una canción individual
struct Cancion {
	Artista *artista;     // Creador o intérprete principal
	char *nombre;         // Título de la canción
	int duracion;         // Duración en segundos
	char *archivo;        // Ruta al archivo de audio
	int popularidad;      // Índice de popularidad
	int reproducciones;   // Número total de reproducciones
};

// Cola de reproducción que combina canciones y anuncios
struct ColaReproduccion {
	Cola canciones;   // Cola de Cancion*
	Cola anuncios;    // Cola de Anuncio*
};

// Representa un anuncio publicitario
struct Anuncio {
	char *anunciante;  // Empresa o anunciante
	char *texto;       // Mensaje del anuncio
};

#endif // ESTRUCTURAS_DB_H