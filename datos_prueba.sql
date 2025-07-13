INSERT INTO Usuarios (username, email, password, nickname, pais, plan, tiempo_escuchado, cantidad_anuncios) VALUES
("alice123", "alice@mail.com", "pass1", "Alice", "Ecuador", 1, 3600, 5),
("bob321", "bob@mail.com", "pass2", "Bob", "Ecuador", 0, 1800, 2),
("carla_music", "carla@mail.com", "pass3", "Carla", "Colombia", 1, 5400, 0),
("Yoasobi", "nose@mail.com", "pass4", "Yovfesd", "Japón", 0, 0, 0),
("eva88", "eva@mail.com", "pass5", "Eva", "Argentina", 1, 0, 0),
("chayanne123", "chayanne@mail.com", "pass6", "Chayanne", "Puerto Rico", 1, 7200, 3),
("pableis", "pacondo@mail.com", "pass7", "Pableis", "Perú", 1,1000,0 ),
("yuki_san", "alexM@mail.com", "pass8", "Yuki", "Japón", 0, 550, 4),
("sneax777", "ronnyp@mail.com", "pass9", "Sneax", "Estados Unidos", 1, 3000, 1),
("jimbo10k", "jimbogod@mail.com", "pass10", "Jimbo", "Canadá", 0, 200, 2),
("Pol", "paulq@mail.com", "pass11", "PolElPulpo", "España", 1, 4000, 3);

INSERT INTO Amigos (id_usuario_1, id_usuario_2) VALUES
(1, 2),
(1, 3),
(1, 6),
(2, 5),
(2, 7),
(2, 4),
(3, 6),
(3, 9),
(4, 7),
(5, 8),
(5, 11),
(6, 9),
(7, 10),
(8, 11),
(9, 1),
(10, 2),
(10, 4),
(11, 3);

INSERT INTO Artistas (id_usuario, nombre) VALUES
(2, "Bob Oficial"),
(3, "Carla Music"),
(4, "DaveBeat"),
(6, "Chayanne"),
(7, "PaCondoo"),
(8, "SneaxMusic"),
(10, "JJimbo");

INSERT INTO Albumes (id_artista, nombre, fecha_creacion) VALUES
(2, "Primera Ola", "2013-10-23"), /* 2 canciones */
(3, "Sueños", "2009-11-14"), /* 3 canciones */
(4, "Idol", "2016-09-29"), /* 3 canciones */
(4, "Alter Ego", "2019-03-21"), /* 4 canciones */
(6, "Alegria", "2009-10-02"), /* 2 canciones */
(7, "Vibras", "2015-10-15"), /* 4 canciones */
(7, "Abismo", "2022-02-14"), /* 2 canciones */
(7, "Guias al sol", "2014-03-16"), /* 2 canciones */
(8, "Eclipse", "2013-03-01"), /* 2 canciones */
(8, "Primavera", "2015-10-23"), /* 3 canciones */
(8, "Conexión", "2019-04-03"), /* 2 canciones */
(10, "Latinaje", "2011-09-16"), /* 3 canciones */
(10, "Eterna", "2013-07-03"); /* 4 canciones */


INSERT INTO Canciones (id_album, nombre, genero, fecha_publicacion, duracion, url, popularidad, reproducciones) VALUES
(3, "Idol", "Pop", "2023-04-12", 225, "PgBvV9ofjmA", 1564, 2000),
(13, "Temperatura", "Reggaeton", "2013-07-03", 237, "J7nc5xQCWUc", 234, 1056),
(13, "Caviar", "Reggaeton", "2017-03-11", 289, "tvj5Fpok9bY", 764, 652),
(7, "Viernes 13", "Balada", "2022-02-14", 214, "Cwevy_TS8xM", 432, 800),
(4, "On My Way", "Electronica", "2019-03-21", 216, "dhYOPzcsbGM", 697, 1097),
(6, "Los Malaventurados No Lloran", "Rock", "2020-11-16", 216, "wvR7EtLuJfg", 542, 711),
(12, "Me Va a Costar", "Pop", "2023-12-06", 228, "myAQeeM12Dg", 831, 792),
(4, "Alone Pt. II", "Electronica", "2019-12-27", 245, "HhjHYkPQ8F0", 687, 903),
(4, "Mortals", "Electronica", "2016-12-15", 228, "zFXl1sjTdms", 842, 913),
(1, "Courtesy Call", "Rock", "2014-06-20", 236, "ocpDEOXABWg", 712, 915),
(3, "We Were Young", "Electronica", "2016-09-29", 213, "ljrmC9NogVI", 213, 398),
(6, "Bajo el Agua", "Pop", "2015-10-15", 220, "rMY9oCyrUgQ", 956, 1203),
(2, "Loba", "Pop", "2009-11-14", 229, "kM2lpgF15o0", 905, 1102),
(8, "La Carretera","Bachata","2016-05-19", 237, "GqQeQbGfHEY", 946, 1231),
(8, "Corazón Sin Cara", "Bachata", "2014-03-16", 212, "AuAmLWbReuQ", 732, 845),
(7, "Maldito ChatGPT", "Pop", "2025-06-25", 210, "uvoZexBIou0", 240, 328),
(13, "Reggaetón Lento", "Reggaeton", "2016-10-07", 222, "oDs1boOYiZo", 973, 1164),
(12, "Una Vaina Loca", "Pop", "2011-09-16", 187, "UdztftsoybQ", 204, 365),
(4, "Rise", "Rock", "2022-10-04", 200, "-EwzeJ4avi0", 630, 830),
(12, "Expectativas", "Pop", "2025-06-26", 205, "gp26viNFasw",175, 149),
(2,"Sin Pijama", "Reggaeton", "2018-04-20", 188, "NvIwMtZ-Shg", 907, 721),
(1, "Same Old War", "Rock", "2013-10-23", 253, "ojeNjpogoqk", 652, 863),
(10, "Aplicándola", "Urbano", "2025-04-25", 188, "8NK4fxxzuA8", 301, 185),
(10, "Soleao", "Urbano", "2025-06-03", 218, "IXRFYGtN9Q8", 231, 187),
(11, "Tal Vez", "Hip Hop", "2019-04-03", 264, "VaHFvFTeXTE", 712, 905),
(9, "Maquiavélico", "Rap", "2013-03-01", 286, "R-GzncAvjm8", 842, 1023),
(9, "Pecador", "Rap", "2019-10-31", 306, "dyfmomgvZDw", 564, 712),
(3, "Grateful", "Electronica", "2019-04-22", 182, "qvBxNxiMWO8", 610, 825),
(10, "Se Acabó", "Urbano", "2015-10-23", 221, "25wDWn58-yo", 671, 880),
(2, "La Bicicleta", "Vallenato", "2023-01-25", 226, "pKAOVdnwCdU", 630, 850),
(6, "Ella Baila Sola", "Corrido", "2023-04-26", 165, "jr6VS-KqaP0", 720, 925),
(13, "Sonríele", "Regaeton", "2025-07-09", 190, "XtBXxnmPYwU", 180, 209),
(6, "Bugambilia", "Rock", "2023-10-15", 189, "a1ozTSkE27U", 678, 901),
(11, "Mucho Para Mí", "Hip-Hop", "2021-12-23", 310, "ymJ1svwvpLQ", 680, 910),
(5, "Humanos a Marte", "Pop", "2014-06-23", 220, "zpWVMosq1oM", 391, 430),
(5, "Dejaría Todo", "Pop", "2009-10-02", 286, "A98ASkT5EXY", 325, 456);

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
