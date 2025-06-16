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
	Lista albumes; // Lista de álbumes del artista
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
	Pila canciones;   // Pila de Reproduccion
	int tiempoEscuchado;   // Tiempo total en segundos
	int cantidadAnuncios;  // Cantidad de anuncios escuchados
};

// Tipos de plan disponibles
enum Plan {
	planFree,  // Con anuncios
	planPremium    // Sin anuncios
};

// En realidad esta estructura amigos no es necesaria, ya que la lista de amigos contiene punteros a usuarios
// struct Amigos {
// 	char *nicknameAmigo; // Nombre de usuario del amigo
// 	Lista playlistsAmigo; // Playlist del amigo
// 	int cantidadAmigos; // Cantidad de amigos
// };

// Representa a un usuario de la aplicación
struct Usuario {
	char *correo; 
	char *contrasenia;  
	char *nombre;         // Nombre real (puede haber varios usuarios con el mismo nombre)
	char *nombreUsuario;  // Nombre único de usuario (puede utilizarse para iniciar sesión)
	char *pais;
	Lista playlists;
	Historial historial; 

	// Amigos amigos; // Esta linea se reemplaza por:
	Lista amigos; // Una lista de usuarios (amigos).
	
	Plan plan;            
};

// Representa un anuncio publicitario
struct Anuncio {
	char *anunciante;  // Empresa o anunciante
	char *mensaje;       // Mensaje del anuncio
	int duracion;       // Duración del anuncio en segundos
};

#endif // ESTRUCTURAS_DB_H