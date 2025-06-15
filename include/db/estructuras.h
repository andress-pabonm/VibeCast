#ifndef ESTRUCTURAS_DB_H
#define ESTRUCTURAS_DB_H

#include <utils/nodos.h> // Para Lista, Pila y Cola

// Estructuras para la base de datos de música tipo Spotify
typedef struct Usuario Usuario; 
typedef struct Playlist Playlist; 
typedef struct Cancion Cancion; 
typedef struct Artista Artista;
typedef struct Historial Historial;
typedef enum Plan Plan;
typedef struct Amigos Amigos; 
typedef struct Anuncio Anuncio;

struct Artista {
	char *nombreArtista; // Nombre del artista
	Playlist *album; // Lista de álbumes del artista
};

struct Cancion {
	char *nombreCancion;
	Artista *artista;
	int duracion;  
	char *genero;  
	char fechaLanzamiento[11]; // Formato DD-MM-YYYY
	int popularidad;  // Cuantas veces se guardo en una playlist
	int reproducciones; // Cantidad de reproducciones
	char *archivo;  //Ruta donde se encuentra el archivo de la canción
};

// Lista de canciones personalizadas por un usuario
struct Playlist {
	char *nombrePlaylist;
	char fechaCreacion[11]; // Formato DD-MM-YYYY
	Cancion canciones;  
};

// Historial de un usuario
struct Historial {
	Cancion canciones;   // Pila de Reproduccion
	int tiempoEscuchado;   // Tiempo total en segundos
	int cantidadAnuncios;  // Cantidad de anuncios escuchados
};

// Tipos de plan disponibles
enum Plan {
	planFree,  // Con anuncios
	planPremium    // Sin anuncios
};

struct Amigos {
	char *nicknameAmigo; // Nombre de usuario del amigo
	Playlist *playlistAmigo; // Playlist del amigo
	int cantidadAmigos; // Cantidad de amigos
};

// Representa a un usuario de la aplicación
struct Usuario {
	char *correo; 
	char *contrasenia;  
	char *nombre;        
	char *nombreUsuario;  
	char *pais;
	Playlist *playlists;
	Historial historial; 
	Amigos amigos;
	Plan plan;            
};

// Representa un anuncio publicitario
struct Anuncio {
	char *anunciante;  // Empresa o anunciante
	char *mensaje;       // Mensaje del anuncio
	int duracion;       // Duración del anuncio en segundos
};

#endif // ESTRUCTURAS_DB_H