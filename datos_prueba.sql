INSERT INTO Usuarios (username, email, password, nickname, pais, plan) VALUES
("alice123", "alice@mail.com", "pass1", "Alice", "Ecuador", 1),
("bob321", "bob@mail.com", "pass2", "Bob", "Ecuador", 0),
("carla_music", "carla@mail.com", "pass3", "Carla", "Colombia", 1),
("davebeat", "dave@mail.com", "pass4", "Dave", "México", 0),
("eva88", "eva@mail.com", "pass5", "Eva", "Argentina", 1);

INSERT INTO Amigos (id_usuario_1, id_usuario_2) VALUES
(1, 2),
(1, 3),
(2, 5);

INSERT INTO Artistas (id_usuario, nombre) VALUES
(3, "Carla Music"),
(4, "DaveBeat");

INSERT INTO Albumes (id_artista, nombre, fecha_creacion) VALUES
(3, "Primera Ola", "2024-01-10"),
(4, "Ritmos Urbanos", "2024-02-20");

INSERT INTO Canciones (id_album, nombre, genero, fecha_publicacion, duracion, url, popularidad, reproducciones) VALUES
(1, "Amanecer", "Pop", "2024-01-12", 210, "canciones/carla/amanecer.mp3", 70, 1200),
(1, "Luz del Día", "Pop", "2024-01-15", 195, "canciones/carla/luz.mp3", 60, 900),
(2, "Bit Urbano", "Electrónica", "2024-02-22", 180, "canciones/dave/bit.mp3", 85, 1500),
(2, "Calle y Sonido", "Hip-hop", "2024-02-25", 200, "canciones/dave/calle.mp3", 90, 1800);

INSERT INTO Playlists (id_usuario, nombre) VALUES
(1, "Favoritas de Alice"),
(2, "Lo que suena");

INSERT INTO Playlist_Canciones (id_playlist, id_cancion) VALUES
(1, 1),
(1, 3),
(2, 2),
(2, 4);

INSERT INTO Historiales (id_usuario, tiempo_escuchado, cantidad_anuncios) VALUES
(1, 3600, 5),
(2, 1800, 2),
(3, 5400, 0);

INSERT INTO Reproducciones (id_usuario, id_cancion, fecha_escuchado) VALUES
(1, 1, "2025-06-20 10:00:00"),
(1, 3, "2025-06-20 11:30:00"),
(2, 2, "2025-06-21 09:00:00"),
(2, 4, "2025-06-21 09:05:00"),
(3, 1, "2025-06-22 08:00:00");

INSERT INTO Anuncios (id_usuario, url) VALUES
(2, "ads/plan_basico/ad1.mp3"),
(2, "ads/plan_basico/ad2.mp3"),
(4, "ads/plan_basico/ad3.mp3");
