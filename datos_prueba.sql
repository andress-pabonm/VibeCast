INSERT INTO Usuarios (username, email, password, nickname, pais, plan, tiempo_escuchado, cantidad_anuncios) VALUES
("alice123", "alice@mail.com", "pass1", "Alice", "Ecuador", 1, 0, 0),
("bob321", "bob@mail.com", "pass2", "Bob", "Ecuador", 0, 0, 0),
("carla_music", "carla@mail.com", "pass3", "Carla", "Colombia", 1, 0, 0),
("yoasobi", "yoasobi@mail.com", "pass4", "Jessica", "Japón", 0, 0, 0),
("eva88", "eva@mail.com", "pass5", "Eva", "Argentina", 1, 0, 0),
("chayanne123", "chayanne@mail.com", "pass6", "Chayanne", "Puerto Rico", 1, 0, 0),
("pableis", "pacondo@mail.com", "pass7", "Pableis", "Perú", 1,0, 0),
("yuki_san", "alexM@mail.com", "pass8", "Yuki", "Japón", 0, 0, 0),
("sneax777", "ronnyp@mail.com", "pass9", "Sneax", "Estados Unidos", 1, 0, 0),
("jimbo10k", "jimbogod@mail.com", "pass10", "Jimbo", "Canadá", 0, 0, 0),
("yol", "paulq@mail.com", "pass11", "PolElPulpo", "España", 1, 0, 0),
("arigameplays", "arigame@mail.com", "pass12", "Ari", "Mexico", 1, 0, 0),
("alexp", "alexponce@mail.com", "pass13", "Alex Ponce", "Ecuador", 1, 0, 0),
("jpablo", "juan123@mail.com", "pass14", "JPablo01", "El Salvador", 0, 0, 0),
("reik","reik25@mail.com", "pass15", "Reik", "Mexico", 1, 0, 0),
("chachin29","elbrocoli@mail.com","pass16", "Brocoli123", "Paraguay", 0, 0, 0);


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
(11, 3),
(11,6),
(11,10),
(12,1),
(12,7),
(12,6),
(12,9),
(12,11),
(13,3),
(14,5),
(14,9),
(14,13),
(15,6),
(15,10),
(16, 1),
(16, 9),
(16, 13);

INSERT INTO Artistas (id_usuario, nombre) VALUES
(2, "Bob Oficial"),
(3, "Carla Music"),
(4, "Yoasobi"),
(6, "Chayanne"),
(7, "PaCondoo"),
(8, "SneaxMusic"),
(10, "JJimbo"),
(12, "Ari Bela"),
(13, "Alex Ponce"),
(15, "Reik");

INSERT INTO Albumes (id_artista, nombre, fecha_creacion) VALUES
(2, "Primera Ola", "2013-10-23"), /* 3 canciones */
(3, "Sueños", "2009-11-14"), /* 3 canciones */
(4, "Idol", "2016-09-29"), /* 4 canciones */
(4, "Alter Ego", "2016-12-15"), /* 5 canciones */
(6, "Alegria", "2009-10-02"), /* 2 canciones */
(7, "Vibras", "2015-10-15"), /* 4 canciones */
(7, "Abismo", "2021-09-16"), /* 3 canciones */
(7, "Guias al sol", "2014-03-16"), /* 2 canciones */
(8, "Eclipse", "2013-03-01"), /* 2 canciones */
(8, "Primavera", "2015-10-23"), /* 3 canciones */
(8, "Conexión", "2019-04-03"), /* 3 canciones */
(10, "Latinaje", "2011-09-16"), /* 4 canciones */
(10, "Eterna", "2013-07-03"), /* 4 canciones */
(12, "Ariatopía", "2025-04-22"), /*3 canciones */
(13, "Ser Humano", "2022-02-14"), /*3 canciones */
(15, "Secuencia", "2015-08-11"); /*3 canciones */

INSERT INTO Canciones (id_album, nombre, genero, fecha_publicacion, duracion, url, popularidad, reproducciones) VALUES
(3, "Idol", "Pop", "2023-04-12", 225, "PgBvV9ofjmA", 0, 0),
(13, "Temperatura", "Reggaeton", "2013-07-03", 237, "J7nc5xQCWUc", 0, 0),
(13, "Caviar", "Reggaeton", "2017-03-11", 289, "tvj5Fpok9bY", 0, 0),
(15, "Viernes 13", "Balada", "2022-02-14", 214, "Cwevy_TS8xM", 0, 0),
(4, "On My Way", "Electronica", "2019-03-21", 216, "dhYOPzcsbGM", 0, 0),
(6, "Los Malaventurados No Lloran", "Rock", "2020-11-16", 216, "wvR7EtLuJfg", 0, 0),
(12, "Me Va a Costar", "Pop", "2023-12-06", 228, "myAQeeM12Dg", 0, 0),
(4, "Alone Pt. II", "Electronica", "2019-12-27", 245, "HhjHYkPQ8F0", 0, 0),
(4, "Mortals", "Electronica", "2016-12-15", 228, "zFXl1sjTdms", 0, 0),
(1, "Courtesy Call", "Rock", "2014-06-20", 236, "ocpDEOXABWg", 0, 0),
(3, "We Were Young", "Electronica", "2016-09-29", 213, "ljrmC9NogVI", 0, 0),
(6, "Bajo el Agua", "Pop", "2015-10-15", 220, "rMY9oCyrUgQ", 0, 0),
(2, "Loba", "Pop", "2009-11-14", 229, "kM2lpgF15o0", 0, 0),
(8, "La Carretera","Bachata","2016-05-19", 237, "GqQeQbGfHEY", 0, 0),
(8, "Corazón Sin Cara", "Bachata", "2014-03-16", 212, "AuAmLWbReuQ", 0, 0),
(7, "Maldito ChatGPT", "Pop", "2025-06-25", 210, "uvoZexBIou0", 0, 0),
(13, "Reggaetón Lento", "Reggaeton", "2016-10-07", 222, "oDs1boOYiZo", 0, 0),
(12, "Una Vaina Loca", "Pop", "2011-09-16", 187, "UdztftsoybQ", 0, 0),
(4, "Rise", "Rock", "2022-10-04", 200, "-EwzeJ4avi0", 0, 0),
(12, "Expectativas", "Pop", "2025-06-26", 205, "gp26viNFasw",0, 0),
(2,"Sin Pijama", "Reggaeton", "2018-04-20", 188, "NvIwMtZ-Shg", 0, 0),
(1, "Same Old War", "Rock", "2013-10-23", 253, "ojeNjpogoqk", 0, 0),
(10, "Aplicándola", "Urbano", "2025-04-25", 188, "8NK4fxxzuA8", 0, 0),
(10, "Soleao", "Urbano", "2025-06-03", 218, "IXRFYGtN9Q8", 0, 0),
(11, "Tal Vez", "Hip Hop", "2019-04-03", 264, "VaHFvFTeXTE", 0, 0),
(9, "Maquiavélico", "Rap", "2013-03-01", 286, "R-GzncAvjm8", 0, 0),
(9, "Pecador", "Rap", "2019-10-31", 306, "dyfmomgvZDw", 0, 0),
(3, "Grateful", "Electronica", "2019-04-22", 182, "qvBxNxiMWO8", 0, 0),
(10, "Se Acabó", "Urbano", "2015-10-23", 221, "25wDWn58-yo", 0, 0),
(2, "La Bicicleta", "Vallenato", "2023-01-25", 226, "pKAOVdnwCdU", 0, 0),
(6, "Ella Baila Sola", "Corrido", "2023-04-26", 165, "jr6VS-KqaP0", 0, 0),
(13, "Sonríele", "Regaeton", "2025-07-09", 190, "XtBXxnmPYwU", 0, 0),
(6, "Bugambilia", "Rock", "2023-10-15", 189, "a1ozTSkE27U", 0, 0),
(11, "Mucho Para Mí", "Hip Hop", "2021-12-23", 310, "ymJ1svwvpLQ", 0, 0),
(5, "Humanos a Marte", "Pop", "2014-06-23", 220, "zpWVMosq1oM", 0, 0),
(5, "Dejaría Todo", "Pop", "2009-10-02", 286, "A98ASkT5EXY", 0, 0),
(15, "Fórmula", "Pop", "2023-03-2023", 184, "vYbc8vYR-KU", 0, 0),
(11, "Champain & Vacay", "Hip Hop", "2025-07.12", 184, "fLLIxc4cyoY", 0, 0),
(14, "Pilates", "Trap", "2025-04-22", 166, "aHYvmY3nV5U", 0, 0),
(14, "Online", "Corrido", "2025-05-20", 212, "F-S5O7PBVjs", 0, 0),
(4, "By my side", "Electronica", "2018-09-29", 178 ,"7CGbq5Yt71k", 0, 0),
(15, "Como Ella Ya No Hay", "Pop", "2025-05-22", 220, "MS4u60087cI",0, 0),
(1, "Lobo o Cordero", "Rock", "2017-09-12", 192, "WuuseQIAjPs", 0, 0),
(12, "Khé?", "Pop", "2024-12-05", 216, "kQgTqxAnpuw", 0, 0),
(14, "Gameboy", "Pop", "2025-06-09", 144, "VCn8o6m27JQ", 0, 0),
(3, "King", "Pop", "2020-08-02", 146, "qQMOtxZ6k7c", 0, 0),
(16, "Ya Me Enteré", "Pop", "2016-04-15", 205, "R_7K9AwpgCU", 0, 0),
(16, "Sabes", "Pop", "2015-08-11", 221, "tNTbzidfsNQ", 0, 0),
(16, "Me Duele Amarte", "Pop", "2015-08-12", 193, "N6Ibv_XisqU", 0, 0),
(7, "A Travez De Las Eras", "Balada", "2021-09-16", 181, "SaxfoYr6PAo", 0, 0),
(7, "Todo Esta Ciudad", "Pop","2021-10-29", 317, "7E9-c2Z6adU", 0, 0);

INSERT INTO Anuncios (id_usuario, url) VALUES
(2, "kIhb5pEo_j0"), /* ChatGPT */
(2, "70ArCjJS_nA"), /* DeepSeek */
(2, "hQ1UE5h9N-k"), /* Juguemos league of legends */
(2, "kIhb5pEo_j0"), /* albion online es un mmorpg no lineal CAMBIAR EL LINK*/
(2, "70ArCjJS_nA"),
(2, "hQ1UE5h9N-k"),
(2, "kIhb5pEo_j0"),
(2, "70ArCjJS_nA"),
(2, "hQ1UE5h9N-k"),
(2, "C0fytYVZ_mQ"),

(4, "kIhb5pEo_j0"),
(4, "70ArCjJS_nA"),
(4, "hQ1UE5h9N-k"),
(4, "kIhb5pEo_j0"),
(4, "70ArCjJS_nA"),
(4, "hQ1UE5h9N-k"),
(4, "kIhb5pEo_j0"),
(4, "70ArCjJS_nA"),
(4, "hQ1UE5h9N-k"),
(4, "C0fytYVZ_mQ"),

(8, "kIhb5pEo_j0"),
(8, "70ArCjJS_nA"),
(8, "hQ1UE5h9N-k"),
(8, "kIhb5pEo_j0"),
(8, "70ArCjJS_nA"),
(8, "hQ1UE5h9N-k"),
(8, "kIhb5pEo_j0"),
(8, "70ArCjJS_nA"),
(8, "hQ1UE5h9N-k"),
(8, "C0fytYVZ_mQ"),

(10, "kIhb5pEo_j0"),
(10, "70ArCjJS_nA"),
(10, "hQ1UE5h9N-k"),
(10, "kIhb5pEo_j0"),
(10, "70ArCjJS_nA"),
(10, "hQ1UE5h9N-k"),
(10, "kIhb5pEo_j0"),
(10, "70ArCjJS_nA"),
(10, "hQ1UE5h9N-k"),
(10, "C0fytYVZ_mQ"),

(14, "kIhb5pEo_j0"),
(14, "70ArCjJS_nA"),
(14, "hQ1UE5h9N-k"),
(14, "kIhb5pEo_j0"),
(14, "70ArCjJS_nA"),
(14, "hQ1UE5h9N-k"),
(14, "kIhb5pEo_j0"),
(14, "70ArCjJS_nA"),
(14, "hQ1UE5h9N-k"),
(14, "C0fytYVZ_mQ"),

(16, "kIhb5pEo_j0"),
(16, "70ArCjJS_nA"),
(16, "hQ1UE5h9N-k"),
(16, "kIhb5pEo_j0"),
(16, "70ArCjJS_nA"),
(16, "hQ1UE5h9N-k"),
(16, "kIhb5pEo_j0"),
(16, "70ArCjJS_nA"),
(16, "hQ1UE5h9N-k"),
(16, "C0fytYVZ_mQ");
