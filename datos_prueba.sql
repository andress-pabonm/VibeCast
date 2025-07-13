INSERT INTO Usuarios (username, email, password, nickname, pais, plan, tiempo_escuchado, cantidad_anuncios) VALUES
("alice123", "alice@mail.com", "pass1", "Alice", "Ecuador", 1, 3600, 5),
("bob321", "bob@mail.com", "pass2", "Bob", "Ecuador", 0, 1800, 2),
("carla_music", "carla@mail.com", "pass3", "Carla", "Colombia", 1, 5400, 0),
("Yoasobi", "nose@mail.com", "pass4", "Yovfesd", "Japón", 0, 0, 0),
("eva88", "eva@mail.com", "pass5", "Eva", "Argentina", 1, 0, 0);

INSERT INTO Amigos (id_usuario_1, id_usuario_2) VALUES
(1, 2),
(1, 3),
(2, 5);

INSERT INTO Artistas (id_usuario, nombre) VALUES
(3, "Carla Music"),
(4, "DaveBeat");

INSERT INTO Albumes (id_artista, nombre, fecha_creacion) VALUES
(3, "Primera Ola", "2024-01-10"),
(4, "Idol", "2024-02-20");

INSERT INTO Canciones (id_album, nombre, genero, fecha_publicacion, duracion, url, popularidad, reproducciones) VALUES
(2, "Idol", "Pop", "2023-04-12", 225, "PgBvV9ofjmA", 1564, 2000),
(1, "Temperatura", "Reggaeton", "2013-07-03", 237, "J7nc5xQCWUc", 234, 1056),
(1, "Caviar", "Reggaeton", "2017-03-11", 289, "tvj5Fpok9bY", 764, 652),
(2, "Viernes 13", "Balada", "2022-02-14", 214, "Cwevy_TS8xM", 432, 800),
(2, "On My Way", "Electronica", "2019-03-21", 216, "dhYOPzcsbGM", 697, 1097),
(1, "Los Malaventurados No Lloran", "Rock", "2020-11-16", 216, "wvR7EtLuJfg", 542, 711),
(1, "Me Va a Costar", "Pop", "2023-12-06", 228, "myAQeeM12Dg", 831, 792),
(2, "Alone Pt. II", "Electronica", "2019-12-27", 245, "HhjHYkPQ8F0", 687, 903),
(2, "Mortals", "Electronica", "2016-12-15", 228, "zFXl1sjTdms", 842, 913),
(1, "Courtesy Call", "Rock", "2014-06-20", 236, "ocpDEOXABWg", 712, 915),
(1, "We Were Young", "Electronica", "2016-09-29", 213, "ljrmC9NogVI", 213, 398),
(1, "Bajo el Agua", "Pop", "2015-10-15", 220, "rMY9oCyrUgQ", 956, 1203),
(2, "Loba", "Pop", "2009-11-14", 229, "kM2lpgF15o0", 905, 1102),
(1, "La Carretera","Bachata","2016-05-19", 237, "GqQeQbGfHEY", 946, 1231),
(2, "Corazón Sin Cara", "Bachata", "2014-03-16", 212, "AuAmLWbReuQ", 732, 845),
(2, "Maldito ChatGPT", "Pop", "2025-06-25", 210, "uvoZexBIou0", 240, 328),
(1, "Reggaetón Lento", "Reggaeton", "2016-10-07", 222, "oDs1boOYiZo", 973, 1164),
(2, "Una Vaina Loca", "Pop", "2011-09-16", 187, "UdztftsoybQ", 204, 365),
(2, "Rise", "Rock", "2022-10-04", 200, "-EwzeJ4avi0", 630, 830),
(1, "Expectativas", "Pop", "2025-06-26", 205, "gp26viNFasw",175, 149),
(2, "Sin Pijama", "Reggaeton", "2018-04-20", 188, "NvIwMtZ-Shg", 907, 721),
(2, "Same Old War", "Rock", "2013-10-23", 253, "ojeNjpogoqk", 652, 863),
(1, "Aplicándola", "Urbano", "2025-04-25", 188, "8NK4fxxzuA8", 301, 185),
(2, "Soleao", "Urbano", "2025-06-03", 218, "IXRFYGtN9Q8", 231, 187),
(1, "Tal Vez", "Hip Hop", "2019-04-03", 264, "VaHFvFTeXTE", 712, 905),
(1, "Maquiavélico", "Rap", "2013-03-01", 286, "R-GzncAvjm8", 842, 1023),
(2, "Pecador", "Rap", "2019-10-31", 306, "dyfmomgvZDw", 564, 712),
(1, "Grateful", "Electronica", "2019-04-22", 182, "qvBxNxiMWO8", 610, 825),
(2, "Se Acabó", "Urbano", "2015-10-23", 221, "25wDWn58-yo", 671, 880),
(1, "La Bicicleta", "Vallenato", "2023-01-25", 226, "pKAOVdnwCdU", 630, 850),
(1, "Ella Baila Sola", "Corrido", "2023-04-26", 165, "jr6VS-KqaP0", 720, 925),
(2, "Sonríele", "Regaeton", "2025-07-09", 190, "XtBXxnmPYwU", 180, 209),
(2, "Bugambilia", "Rock", "2023-10-15", 189, "a1ozTSkE27U", 678, 901),
(1, "Mucho Para Mí", "Hip-Hop", "2021-12-23", 310, "ymJ1svwvpLQ", 680, 910),
(2, "Humanos a Marte", "Pop", "2014-06-23", 220, "zpWVMosq1oM", 391, 430),
(2, "Dejaría Todo", "Pop", "2009-10-02", 286, "A98ASkT5EXY", 325, 456);

INSERT INTO Playlists (id_usuario, nombre) VALUES
(1, "Favoritas de Alice"),
(2, "Lo que suena");

INSERT INTO Playlist_Canciones (id_playlist, id_cancion) VALUES
(1, 1),
(1, 3),
(2, 2),
(2, 4);

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
