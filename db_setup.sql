CREATE TABLE IF NOT EXISTS Usuarios (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    email TEXT NOT NULL UNIQUE,
    password TEXT NOT NULL,
    nickname TEXT NOT NULL,
    pais TEXT NOT NULL,
    plan INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS Amigos (
    id_usuario_1 INTEGER NOT NULL,
    id_usuario_2 INTEGER NOT NULL,
    PRIMARY KEY (id_usuario_1, id_usuario_2),
    FOREIGN KEY (id_usuario_1) REFERENCES Usuarios(id),
    FOREIGN KEY (id_usuario_2) REFERENCES Usuarios(id)
);

CREATE TABLE IF NOT EXISTS Artistas (
    id_usuario INTEGER PRIMARY KEY,
    nombre TEXT NOT NULL UNIQUE,
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id)
);

CREATE TABLE IF NOT EXISTS Albumes (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_artista INTEGER NOT NULL,
    nombre TEXT NOT NULL,
    fecha_creacion TEXT NOT NULL,
    UNIQUE (id_artista, nombre),
    FOREIGN KEY (id_artista) REFERENCES Artistas(id_usuario)
);

CREATE TABLE IF NOT EXISTS Canciones (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_album INTEGER NOT NULL,
    nombre TEXT NOT NULL,
    genero TEXT NOT NULL,
    fecha_publicacion TEXT NOT NULL,
    duracion INTEGER NOT NULL,
    url TEXT NOT NULL,
    popularidad INTEGER NOT NULL DEFAULT 0,
    reproducciones INTEGER NOT NULL DEFAULT 0,
    UNIQUE (id_album, nombre),
    FOREIGN KEY (id_album) REFERENCES Albumes(id)
);

CREATE TABLE IF NOT EXISTS Playlists (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_usuario INTEGER NOT NULL,
    nombre TEXT NOT NULL,
    UNIQUE (id_usuario, nombre),
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id)
);

CREATE TABLE IF NOT EXISTS Playlist_Canciones (
    id_playlist INTEGER NOT NULL,
    id_cancion INTEGER NOT NULL,
    PRIMARY KEY (id_playlist, id_cancion),
    FOREIGN KEY (id_playlist) REFERENCES Playlists(id),
    FOREIGN KEY (id_cancion) REFERENCES Canciones(id)
);

CREATE TABLE IF NOT EXISTS Historiales (
    id_usuario INTEGER PRIMARY KEY,
    tiempo_escuchado INTEGER NOT NULL DEFAULT 0,
    cantidad_anuncios INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id)
);

CREATE TABLE IF NOT EXISTS Reproducciones (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_usuario INTEGER NOT NULL,
    id_cancion INTEGER NOT NULL,
    fecha_escuchado TEXT NOT NULL,
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id),
    FOREIGN KEY (id_cancion) REFERENCES Canciones(id)
);

CREATE TABLE IF NOT EXISTS Anuncios (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_usuario INTEGER NOT NULL,
    url TEXT NOT NULL,
    FOREIGN KEY (id_usuario) REFERENCES Usuarios(id)
);
