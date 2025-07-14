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
    async function loadSongs() {
      const songsContainer = document.getElementById("songs-container");
      if (!songsContainer) return;

      try {
        const res = await window.obtener_canciones();
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
          console.warn("No se pudo cargar canciones:", res.message || "Respuesta inválida."
          );
        }
      } catch (err) {
        console.error("Error al cargar canciones:", err);
      }
    }

    async function showPlaylistsModal(song) {
      try {
        const res = await window.obtener_playlists();
        console.log("get_playlists():", res);

        if (res.status === "ok" && res.type === "json") {
          const modal = document.createElement("div");
          modal.className = "playlist-modal";
          modal.innerHTML = `
            <div class="modal-content">
              <div class="modal-header">
                <h3>Añadir a playlist</h3>
                <span class="close-modal">&times;</span>
              </div>
              <div class="modal-body">
                <p>Añadir "${song.title}" a:</p>
                <ul class="playlist-list"></ul>
              </div>
            </div>
          `;

          document.body.appendChild(modal);

          const playlistList = modal.querySelector(".playlist-list");
          res.data.forEach(playlist => {
            const li = document.createElement("li");
            li.textContent = playlist.name;
            li.addEventListener("click", async () => {
              try {
                const addRes = await window.agregar_cancion_playlist({
                  playlist_id: playlist.id,
                  song_id: song.id
                });

                if (addRes.status === "ok") {
                  alert(`Canción añadida a ${playlist.name}`);
                  modal.remove();
                } else {
                  alert("Error al añadir a playlist: " + (addRes.message || "Error desconocido"));
                }
              } catch (err) {
                console.error("Error al añadir a playlist:", err);
                alert("Error al añadir a playlist");
              }
            });
            playlistList.appendChild(li);
          });

          // Cerrar modal al hacer clic en la X o fuera del contenido
          modal.querySelector(".close-modal").addEventListener("click", () => modal.remove());
          modal.addEventListener("click", (e) => {
            if (e.target === modal) modal.remove();
          });
        }
      } catch (err) {
        console.error("Error al cargar playlists:", err);
      }
    }

    async function loadRecommendations() {
      const recommendationsContainer = document.getElementById(
        "recommendations-container"
      );
      if (!recommendationsContainer) return;

      try {
        const res = await window.obtener_recomendaciones(); // ← reemplaza mock
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
            <span>${formatTime(song.duration)}</span>
            <i class="fas fa-play"></i>
            <i class="fas fa-plus"></i>
            <i class="fas fa-plus-circle add-to-playlist" title="Añadir a playlist"></i>
        </div>
      `;

      return div;
    }

    function createRecommendationMenu(song, element) {
      const menu = document.createElement("div");
      menu.className = "recommendation-menu";
      menu.innerHTML = `
        <div class="menu-options">
          <div class="menu-option" data-action="play"><i class="fas fa-play"></i> Reproducir</div>
          <div class="menu-option" data-action="add"><i class="fas fa-plus"></i> Añadir a cola</div>
          <div class="menu-option" data-action="add-to-playlist"><i class="fas fa-plus-circle"></i> Añadir a playlist</div>
        </div>
      `;

      // Posicionar el menú debajo del elemento
      const rect = element.getBoundingClientRect();
      menu.style.position = 'absolute';
      menu.style.top = `${rect.bottom + window.scrollY}px`;
      menu.style.left = `${rect.left + window.scrollX}px`;
      menu.style.transform = 'translateX(-50%)';

      document.body.appendChild(menu);

      // Manejar clics en las opciones
      menu.addEventListener("click", (e) => {
        const option = e.target.closest(".menu-option");
        if (!option) return;

        const action = option.dataset.action;

        switch (action) {
          case "play":
            MusicPlayer.addSongToQueue(song, true);
            break;
          case "add":
            MusicPlayer.addSongToQueue(song, false);
            break;
          case "add-to-playlist":
            showPlaylistsModal(song);
            break;
        }

        menu.remove();
      });

      // Cerrar el menú al hacer clic fuera
      setTimeout(() => {
        const closeMenu = (e) => {
          if (!menu.contains(e.target) && e.target !== element) {
            menu.remove();
            document.removeEventListener("click", closeMenu);
          }
        };
        document.addEventListener("click", closeMenu);
      }, 0);
    }

    // Modifica createRecommendationElement para incluir el menú de 3 puntos
    function createRecommendationElement(song) {
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
          <span>${formatTime(song.duration)}</span>
          <i class="fas fa-ellipsis-v menu-icon"></i>
        </div>
      `;

      // Agregar evento para el menú de 3 puntos
      div.querySelector(".menu-icon").addEventListener("click", (e) => {
        e.stopPropagation();
        createRecommendationMenu(song, e.target);
      });

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

          MusicPlayer.addSongToQueue(song, true);
        }

        if (e.target.classList.contains("fa-plus")) {
          console.log("➕ Encolando (sin reproducir):", song.title);

          MusicPlayer.addSongToQueue(song, false);
        }

        if (e.target.classList.contains("add-to-playlist")) {
          e.stopPropagation();
          showPlaylistsModal(song);
        }
      });
    }

    return function () {
      loadSongs();
      loadRecommendations();
      setupSongClickEvents();
    };
  })(),
};
