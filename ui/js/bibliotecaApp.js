window.views = window.views || {};

window.views.biblioteca = {
  title: `<i class="fas fa-music"></i>Biblioteca`,
  html: `
      <header class="main-header">
        <div class="search-bar">
            <i class="fas fa-search search-icon"></i>
            <input type="text" placeholder="Buscar en tus playlists..." class="search-input" />
        </div>
            <button class="new-playlist-btn" id="newPlaylistBtn">
              <i class="fas fa-plus"></i> Nueva playlist
        </button>
    </header>

    <div class="library-container">
        <!-- Sección de Playlists -->
        <section class="playlists-section">
            <h2 class="section-title">
                <i class="fas fa-list"></i> Tus Playlists
            </h2>
            <div class="playlists-grid" id="playlistsContainer">
                <!-- Playlists se cargarán dinámicamente -->
            </div>
        </section>

      <!-- Sección de Canciones -->
      <section class="songs-section hidden" id="songsSection">
          <div class="songs-header">
              <button class="back-btn" id="backToPlaylists">
                  <i class="fas fa-arrow-left"></i>
              </button>
              <h2 id="currentPlaylistTitle"></h2>
              <button class="action-btn" id="addToQueueBtn">
                  <i class="fas fa-plus-circle"></i> Añadir a cola
              </button>
          </div>
        <div class="song-list" id="playlistSongsContainer">
            <!-- Canciones de la playlist seleccionada -->
        </div>
      </section>
    </div>

    <!-- Modal para nueva playlist -->
    <div class="modal hidden" id="newPlaylistModal">
        <div class="modal-content">
            <span class="close-modal">&times;</span>
            <h3>Crear nueva playlist</h3>
            <input type="text" id="playlistNameInput" placeholder="Nombre de la playlist" />
            <button id="createPlaylistBtn">Crear</button>
        </div>
    </div>
  `,
  style: "styles/bibliotecaStyles.css",
  init: function () {
    function init() {
      loadPlaylists();
      setupEventListeners();
    };

    // Variables globales
    const playlistsContainer = document.getElementById('playlistsContainer');
    const songsSection = document.getElementById('songsSection');
    const playlistSongsContainer = document.getElementById('playlistSongsContainer');
    const currentPlaylistTitle = document.getElementById('currentPlaylistTitle');
    const newPlaylistBtn = document.getElementById('newPlaylistBtn');
    const newPlaylistModal = document.getElementById('newPlaylistModal');
    const closeModal = document.querySelectorAll('.close-modal');
    const createPlaylistBtn = document.getElementById('createPlaylistBtn');
    const playlistNameInput = document.getElementById('playlistNameInput');
    const backToPlaylists = document.getElementById('backToPlaylists');
    const editPlaylistModal = document.getElementById('editPlaylistModal');
    const savePlaylistBtn = document.getElementById('savePlaylistBtn');
    const deletePlaylistBtn = document.getElementById('deletePlaylistBtn');
    const editPlaylistNameInput = document.getElementById('editPlaylistNameInput');
    const addToQueueBtn = document.getElementById('addToQueueBtn');
    let currentPlaylistId = null;
    let currentPlaylistSongs = {};
    let currentPlaylists = [];

    async function loadPlaylists() {
      try {
        const res = await window.get_playlists();
        console.log("get_playlists():", res);

        if (res.status === "ok" && res.type === "json") {
          currentPlaylists = res.data.playlists;
          currentPlaylistSongs = res.data.playlistSongs;
          renderPlaylists(currentPlaylists);
        }
      } catch (error) {
        console.error("Error al cargar playlists:", error);
        alert("Error al cargar playlists");
      }
    }

    function renderPlaylists(playlists) {
      playlistsContainer.innerHTML = '';
      playlists.forEach(playlist => {
        const playlistCard = document.createElement('div');
        playlistCard.className = 'playlist-card';
        playlistCard.innerHTML = `
            <div class="playlist-image">
                <div class="playlist-options" data-id="${playlist.id}">
                    <i class="fas fa-ellipsis-h"></i>
                </div>
                <i class="fas fa-music"></i>
            </div>
            <div class="playlist-info">
                <h3 class="playlist-name">${playlist.name}</h3>
                <p class="playlist-song-count">${playlist.songCount} canciones</p>
            </div>
        `;
        playlistCard.addEventListener('click', () => showPlaylistSongs(playlist.id, playlist.name));
        playlistsContainer.appendChild(playlistCard);
      });
    }

    function showPlaylistSongs(playlistId, playlistName) {
      const songs = currentPlaylistSongs[playlistId];
      if (!songs) {
        alert("No se encontraron canciones para esta playlist.");
        return;
      }

      currentPlaylistId = playlistId;
      currentPlaylistTitle.textContent = playlistName;
      playlistSongsContainer.innerHTML = '';

      songs.forEach(song => {
        const songElement = document.createElement('div');
        songElement.className = 'song-item';
        songElement.innerHTML = `
            <div class="song-info">
                <strong>${song.title}</strong>
                <p>${song.artist}</p>
            </div>
            <div class="song-actions">
                <span>${song.duration}</span>
                <i class="fas fa-play play-btn" title="Reproducir"></i>
            </div>
        `;
        songElement.querySelector('.play-btn').addEventListener('click', (e) => {
          e.stopPropagation();
          MusicPlayer.playSong(song);
        });
        playlistSongsContainer.appendChild(songElement);
      });

      document.querySelector('.playlists-section').classList.add('hidden');
      songsSection.classList.remove('hidden');
    }

    function setupEventListeners() {
      backToPlaylists.addEventListener('click', () => {
        songsSection.classList.add('hidden');
        document.querySelector('.playlists-section').classList.remove('hidden');
      });

      newPlaylistBtn.addEventListener('click', () => {
        newPlaylistModal.classList.remove('hidden');
      });

      closeModal.forEach(btn => {
        btn.addEventListener('click', () => {
          newPlaylistModal.classList.add('hidden');
          editPlaylistModal.classList.add('hidden');
        });
      });

      // Crear nueva playlist (esto aún es local y deberías implementar lógica real en el backend)
      createPlaylistBtn.addEventListener('click', () => {
        const playlistName = playlistNameInput.value.trim();
        if (playlistName) {
          alert("Funcionalidad de creación de playlist no implementada aún.");
          playlistNameInput.value = '';
          newPlaylistModal.classList.add('hidden');
        }
      });

      // Guardar cambios en playlist (solo nombre, se requiere lógica en backend)
      savePlaylistBtn.addEventListener('click', () => {
        const playlistName = editPlaylistNameInput.value.trim();
        if (playlistName && currentPlaylistId) {
          const playlist = currentPlaylists.find(p => p.id === currentPlaylistId);
          if (playlist) {
            alert("Funcionalidad de edición no implementada aún.");
            editPlaylistModal.classList.add('hidden');
          }
        }
      });

      // Eliminar playlist (falta lógica real)
      deletePlaylistBtn.addEventListener('click', () => {
        if (currentPlaylistId && confirm('¿Estás seguro de eliminar esta playlist?')) {
          alert("Funcionalidad de eliminación no implementada aún.");
          editPlaylistModal.classList.add('hidden');
          songsSection.classList.add('hidden');
          document.querySelector('.playlists-section').classList.remove('hidden');
          loadPlaylists();
        }
      });

      // Añadir toda la playlist a la cola
      addToQueueBtn.addEventListener('click', () => {
        if (currentPlaylistId) {
          const songs = currentPlaylistSongs[currentPlaylistId] || [];
          if (songs.length > 0) {
            MusicPlayer.addSongsToQueue(songs);
            alert(`Se añadieron ${songs.length} canciones a la cola`);
          }
        }
      });
    }

    init();
  },
};
