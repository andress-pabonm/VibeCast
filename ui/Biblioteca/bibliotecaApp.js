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

document.addEventListener("DOMContentLoaded", () => {
    loadPlaylists();
    setupEventListeners();
});

async function loadPlaylists() {
    try {
        const res = await window.get_playlists();
        console.log("get_playlists():", res);

        if (res.status === "ok" && res.type === "json") {
            renderPlaylists(res.data.playlists);
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

async function showPlaylistSongs(playlistId, playlistName) {
    try {
        const res = await window.get_playlist_songs(playlistId);
        if (res.status === "ok" && res.type === "json") {
            currentPlaylistId = playlistId;
            currentPlaylistTitle.textContent = playlistName;
            playlistSongsContainer.innerHTML = '';

            res.data.forEach(song => {
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
    } catch (error) {
        console.error("Error al cargar canciones:", error);
        alert("Error al cargar canciones");
    }
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

    // Crear nueva playlist
    createPlaylistBtn.addEventListener('click', () => {
        const playlistName = playlistNameInput.value.trim();
        if (playlistName) {
            // Crear nueva playlist
            const newId = Math.max(...mockData.playlists.map(p => p.id), 0) + 1;
            mockData.playlists.push({
                id: newId,
                name: playlistName,
                songCount: 0
            });

            mockData.playlistSongs[newId] = [];

            playlistNameInput.value = '';
            newPlaylistModal.classList.add('hidden');
        }
    });

    // Guardar cambios en playlist
    savePlaylistBtn.addEventListener('click', () => {
        const playlistName = editPlaylistNameInput.value.trim();
        if (playlistName && currentPlaylistId) {
            const playlist = mockData.playlists.find(p => p.id === currentPlaylistId);
            if (playlist) {
                playlist.name = playlistName;
                currentPlaylistTitle.textContent = playlistName;
                loadPlaylists();
                editPlaylistModal.classList.add('hidden');
            }
        }
    });

    // Eliminar playlist
    deletePlaylistBtn.addEventListener('click', () => {
        if (currentPlaylistId && confirm('¿Estás seguro de eliminar esta playlist?')) {
            mockData.playlists = mockData.playlists.filter(p => p.id !== currentPlaylistId);
            delete mockData.playlistSongs[currentPlaylistId];
            editPlaylistModal.classList.add('hidden');
            songsSection.classList.add('hidden');
            document.querySelector('.playlists-section').classList.remove('hidden');
            loadPlaylists();
        }
    });

    // Añadir toda la playlist a la cola
    addToQueueBtn.addEventListener('click', () => {
        if (currentPlaylistId) {
            const songs = mockData.playlistSongs[currentPlaylistId] || [];
            if (songs.length > 0) {
                MusicPlayer.addSongsToQueue(songs);
                alert(`Se añadieron ${songs.length} canciones a la cola`);
            }
        }
    });
}