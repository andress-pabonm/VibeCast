window.views = window.views || {};

window.views.historial = {
  title: `<i class="fas fa-history"></i>Historial`,
  html: `
        <header class="main-header">
        <div class="search-bar">
            <i class="fas fa-search search-icon"></i>
            <input type="text" placeholder="Buscar en tu historial..." class="search-input">
        </div>
    </header> 

    <div class="history-container">
        <!-- Lista de historial -->
        <section class="history-list">
            <h2 class="section-title"><i class="fas fa-clock"></i> Recientemente escuchado</h2>
            <div class="song-list" id="historyItemsContainer">
                <!-- Historial se cargará dinámicamente -->
            </div>
        </section>
    </div>
  `,
  init: function () {
    // Elementos del DOM
    const historyContainer = document.getElementById("historyItemsContainer");

    // Cargar historial
    async function loadHistory() {
      historyContainer.innerHTML = "";

      try {
        const res = await window.obtener_historial();

        if (res.status !== "ok") {
          throw new Error(res.message);
        }

        console.log(res.message);

        let filteredHistory = res.data;

        console.log(filteredHistory);

        filteredHistory.sort((a, b) => b.playedAt - a.playedAt);

        filteredHistory.forEach((item) => {
          const historyElement = document.createElement("div");
          historyElement.className = "history-item";
          historyElement.dataset.id = item.songId;
          historyElement.dataset.title = item.title;
          historyElement.innerHTML = `
                <div class="history-info">
                    <div class="song-title">${item.title}</div>
                    <div class="play-date">
                        <i class="far fa-clock"></i>
                        ${item.playedAt}
                    </div>
                </div>
                <div class="history-actions">
                    <span>${formatTime(item.duration)}</span>
                    <i class="fas fa-play"></i>
                    <i class="fas fa-plus"></i>
                    <i class="fas fa-plus-circle add-to-playlist" title="Añadir a playlist"></i>
                </div>
            `;

          historyContainer.appendChild(historyElement);
        });
      } catch (error) {
        console.warn(error.message);
      }
    }

    function setupSongClickEvents() {
      document.addEventListener("click", async (e) => {
        e.stopPropagation();
        const item = e.target.closest(".history-item");
        if (!item) return;

        const song = {
          id: parseInt(item.dataset.id),
          title: item.dataset.title,
          artist: item.dataset.artist,
        };

        if (e.target.classList.contains("fa-play")) {
          console.log("▶ Encolando y reproduciendo:", song.title);
          await MusicPlayer.addSongToQueue(song, true);
        } else if (e.target.classList.contains("fa-plus")) {
          console.log("➕ Encolando (sin reproducir):", song.title);
          await MusicPlayer.addSongToQueue(song, false);
        } else if (e.target.classList.contains("fa-plus-circle")) {
          await showPlaylistsModal(song);
        }
      });
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
                <p class="close-modal">&times;</p>
              </div>
              <div class="modal-body">
                <p>Añadir "${song.title}" a:</p>
                <ul class="playlist-list"></ul>
              </div>
            </div>
          `;

          document.body.appendChild(modal);

          const playlistList = modal.querySelector(".playlist-list");
          res.data.playlists.forEach(playlist => {
            const li = document.createElement("li");
            li.textContent = playlist.name;
            li.addEventListener("click", async () => {
              try {
                const res = await window.agregar_a_playlist(playlist.id, song.id);

                if (res.status === "ok") {
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

    loadHistory();
    setupSongClickEvents();
  },
};
