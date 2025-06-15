#ifndef ESTRUCTURAS_DB_H
#define ESTRUCTURAS_DB_H

#include <utils/nodos.h> // Para Lista, Pila y Cola

// Estructuras para la base de datos de música tipo Spotify

// Representa a un usuario de la aplicación
typedef struct Usuario {
	char *correo; 
	char *contrasenia;  
	char *nombre;        
	char *nombreUsuario;  
	char *pais;
	struct Playlist playlists;
	struct Historial historial; 
	struct Amigos amigos;
	enum Plan plan;            
}Usuario;

// Lista de canciones personalizadas por un usuario
typedef struct Playlist {
	char *nombrePlaylist;
	char *fechaCreacion; // Formato DD-MM-YYYY
	struct Cancion canciones;  
}Playlist;

typedef struct Cancion {
	char *nombreCancion;
	struct Artista artista;
	int duracion;  
	char *genero;  
	char *fechaLanzamiento; //Formato DD-MM-YYYY
	int popularidad;  // Cuantas veces se guardo en una playlist
	int reproducciones; // Cantidad de reproducciones
	char *archivo;  //Ruta donde se encuentra el archivo de la canción
}Cancion;

// Representa a un creador o interprete musical
typedef struct Artista {
	char *nombreArtista;  // Nombre del artista
	Playlist albumes;     // Albunes del artista
	//Es de tipo playlist porque un artista puede tiene nombre del album, fecha de lanzamiento y canciones
}Artista;

// Historial de un usuario
typedef struct Historial {
	Cancion canciones;   // Pila de Reproduccion
	int tiempoEscuchado;   // Tiempo total en segundos
	int cantidadAnuncios;  // Cantidad de anuncios escuchados
}Historial;

// Tipos de plan disponibles
typedef enum Plan {
	planFree,  // Con anuncios
	planPremium    // Sin anuncios
}Plan;

// Representa un anuncio publicitario
typedef struct Anuncio {
	char *anunciante;  // Empresa o anunciante
	char *mensaje;       // Mensaje del anuncio
	int duracion;       // Duración del anuncio en segundos
}Anuncio;

typedef struct Amigos {
	char nicknameAmigo; // Nombre de usuario del amigo
	Playlist playlistAmigo; // Playlist del amigo
	int cantidadAmigos; // Cantidad de amigos
}Amigos;

#endif // ESTRUCTURAS_DB_H