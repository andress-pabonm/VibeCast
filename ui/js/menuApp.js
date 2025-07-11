window.views = window.views || {};
window.views.inicio = {
  title: `<i class="fas fa-home"></i>Inicio`,
  html: `
    <header class="main-header">
    <div class="search-bar">
      <i class="fas fa-search search-icon"></i>
      <input type="text" placeholder="Buscar canciones, artistas..." class="search-input" />
    </div>
  </header>

  <div class="content-grid">
    <!-- Canciones de la base de datos -->
    <section class="card-section database-songs">
      <h2 class="section-title">
        <i class="fas fa-database"></i>Explorar Canciones
      </h2>
      <div class="song-list" id="songs-container">
        <!-- Canciones se cargarán dinámicamente -->
      </div>
    </section>

    <!-- Recomendaciones de amigos -->
    <section class="card-section friend-recommendations">
      <h2 class="section-title">
        <i class="fas fa-heart"></i>Recomendaciones
      </h2>
      <div class="song-list" id="recommendations-container">
        <!-- Canciones recomendadas se cargarán dinámicamente -->
      </div>
    </section>

    <!-- Estadisticas del usuario -->
    <section class="card-section friends">
      <h2 class="section-title">
        <i class="fas fa-chart-line"></i>Estadisticas
      </h2>
      <div class="friends-list" id="friends-container">
        <!-- Estadisticas se cargarán dinámicamente -->
      </div>
    </section>
  </div>
  `,
  init: (function () {
    /**
     * Verifica si hay sesión iniciada.
     */
    async function checkUserLoggedIn() {
      try {
        const res = await window.is_logged_in();
        console.log("is_logged_in():", res);

        if (
          res.status !== "ok" ||
          res.type !== "boolean" ||
          res.data !== true
        ) {
          window.location.replace("../Login/index.html");
        }
      } catch (err) {
        console.error("Error al verificar sesión:", err);
      }
    }

    /**
     * Carga las canciones disponibles desde el backend.
     */
    async function loadSongs() {
      const songsContainer = document.getElementById("songs-container");
      if (!songsContainer) return;

      try {
        const res = await window.get_canciones(); // ← reemplaza mock
        console.log("canciones():", res);

        if (
          res.status === "ok" &&
          res.type === "array" &&
          Array.isArray(res.data)
        ) {
          res.data.forEach((song) => {
            const songElement = createSongElement(song);
            songsContainer.appendChild(songElement);
          });
        } else {
          console.warn(
            "No se pudo cargar canciones:",
            res.message || "Respuesta inválida."
          );
        }
      } catch (err) {
        console.error("Error al cargar canciones:", err);
      }
    }

    /**
     * Carga las canciones recomendadas desde el backend.
     */
    async function loadRecommendations() {
      const recommendationsContainer = document.getElementById(
        "recommendations-container"
      );
      if (!recommendationsContainer) return;

      try {
        const res = await window.get_recomendaciones(); // ← reemplaza mock
        console.log("recomendaciones():", res);

        if (
          res.status === "ok" &&
          res.type === "array" &&
          Array.isArray(res.data)
        ) {
          res.data.forEach((song) => {
            const songElement = createRecommendationElement(song);
            recommendationsContainer.appendChild(songElement);
          });
        } else {
          console.warn(
            "No se pudo cargar recomendaciones:",
            res.message || "Respuesta inválida."
          );
        }
      } catch (err) {
        console.error("Error al cargar recomendaciones:", err);
      }
    }

    /**
     * Crea un elemento DOM para una canción.
     */
    function createSongElement(song) {
      const div = document.createElement("div");
      div.className = "song-item";
      div.dataset.id = song.id;
      div.dataset.title = song.title;
      div.dataset.artist = song.artist;

      div.innerHTML = `
    <div class="song-info">
        <strong>${song.title}</strong>
        <p>${song.artist}</p>
    </div>
    <div class="song-actions">
        <span>${song.duration}</span>
        <i class="fas fa-play"></i>
        <i class="fas fa-plus"></i>
    </div>`;
      return div;
    }

    /**
     * Crea un elemento DOM para una canción recomendada.
     */
    function createRecommendationElement(song) {
      const div = document.createElement("div");
      div.className = "song-item";
      div.innerHTML = `
    <div class="song-info">
        <strong>${song.title}</strong>
        <p>${song.artist} · Recomendado por ${song.friend}</p>
    </div>
    <div class="song-actions">
        <span>${song.duration}</span>
        <i class="fas fa-play"></i>
    </div>`;
      return div;
    }

    /**
     * Configura eventos para manejar clics en canciones.
     */
    function setupSongClickEvents() {
      document.addEventListener("click", async (e) => {
        const item = e.target.closest(".song-item");
        if (!item) return;

        const song = {
          id: parseInt(item.dataset.id),
          title: item.dataset.title,
          artist: item.dataset.artist,
        };

        if (e.target.classList.contains("fa-play")) {
          console.log("▶ Encolando y reproduciendo:", song.title);

          MusicPlayer.addSongsToQueue(song, true);
        }

        if (e.target.classList.contains("fa-plus")) {
          console.log("➕ Encolando (sin reproducir):", song.title);

          MusicPlayer.addSongsToQueue(song, false);
        }
      });
    }

    return function () {
      checkUserLoggedIn();

      loadSongs();
      loadRecommendations();

      setupSongClickEvents();
    };
  })(),
};
