#ifndef ESTRUCTURAS_DB_H
#define ESTRUCTURAS_DB_H

#include <utils/nodos.h> // Para Lista, Pila y Cola

// Estructuras para la base de datos de música tipo Spotify

typedef struct Usuario {
	char *correo;     
	char *contrasenia;  
	char *nombre;        
	char *nombreUsuario;  
	char *pais;        
	Plan plan;          
	Historial historial;  
	Lista playlists;   
	Lista amigos;      
	Artista *perfilArtista; 
}Usuario;

// Tipos de plan disponibles
typedef enum Plan {
	PLAN_FREEMIUM,  // Con anuncios
	PLAN_PREMIUM    // Sin anuncios
}Plan;

// Historial de un usuario (canciones escuchadas, tiempo, anuncios)
typedef struct Historial {
	Pila reproducciones;   // Pila de Reproduccion*
	int tiempoEscuchado;   // Tiempo total en segundos
	int cantidadAnuncios;  // Cantidad de anuncios escuchados
}Historial;

// Representa una reproducción individual
typedef struct Reproduccion {
	Cancion *cancion;  // Canción reproducida
	char *fecha;       // Fecha de reproducción (formato YYYY-MM-DD)
}Reproduccion;

// Lista de canciones personalizadas por un usuario
typedef struct Playlist {
	char *nombre;      // Nombre de la playlist
	Lista canciones;   // Lista de Cancion*
}Playlist;

// Representa a un artista musical
typedef struct Artista {
	char *nombre;      // Nombre artístico
	Lista albumes;     // Lista de Album*
}Artista;

// Representa un álbum musical
typedef struct Album {
	char *nombre;      // Nombre del álbum
	char *fecha;       // Fecha de lanzamiento
	Lista canciones;   // Lista de Cancion*
}Album;

// Representa una canción individual
typedef struct Cancion {
	Artista *artista;     // Creador o intérprete principal
	char *nombre;         // Título de la canción
	int duracion;         // Duración en segundos
	char *archivo;        // Ruta al archivo de audio
	int popularidad;      // Índice de popularidad
	int reproducciones;   // Número total de reproducciones
}Cancion;

// Cola de reproducción que combina canciones y anuncios
typedef struct ColaReproduccion {
	Cola canciones;   // Cola de Cancion*
	Cola anuncios;    // Cola de Anuncio*
}colaReproduccion;

// Representa un anuncio publicitario
typedef struct Anuncio {
	char *anunciante;  // Empresa o anunciante
	char *texto;       // Mensaje del anuncio
}Anuncio;

#endif // ESTRUCTURAS_DB_H