//Andres prueba la parte grafica y luego modifica o no se XD
//Esta agregado para q cree una playlist desde la interfaz pero no crea en si la playlist en la interfaz

document.addEventListener('DOMContentLoaded', () => {
    // Datos de ejemplo
    const mockData = {
        playlists: [
            { id: 1, name: "Favoritas", songCount: 24 },
            { id: 2, name: "Rock Clásico", songCount: 15 },
            { id: 3, name: "Para estudiar", songCount: 8 },
            { id: 4, name: "Éxitos 2023", songCount: 32 },
            { id: 5, name: "Mixtape Verano", songCount: 18 },
            { id: 6, name: "Iban 2", songCount: 2}
        ],
        playlistSongs: {
            1: [
                { id: 1, title: "Bohemian Rhapsody", artist: "Queen", duration: "5:55" },
                { id: 2, title: "Imagine", artist: "John Lennon", duration: "3:04" }
            ],
            2: [
                { id: 3, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56" },
                { id: 4, title: "Hotel California", artist: "Eagles", duration: "6:30" }
            ]
        }
    };

    // Elementos del DOM
    const playlistsContainer = document.getElementById('playlistsContainer');
    const songsSection = document.getElementById('songsSection');
    const playlistSongsContainer = document.getElementById('playlistSongsContainer');
    const currentPlaylistTitle = document.getElementById('currentPlaylistTitle');
    const newPlaylistBtn = document.getElementById('newPlaylistBtn');
    const newPlaylistModal = document.getElementById('newPlaylistModal');
    const closeModal = document.querySelector('.close-modal');
    const createPlaylistBtn = document.getElementById('createPlaylistBtn');
    const playlistNameInput = document.getElementById('playlistNameInput');
    const backToPlaylists = document.getElementById('backToPlaylists');

    // Cargar playlists
    function loadPlaylists() {
        playlistsContainer.innerHTML = '';
        mockData.playlists.forEach(playlist => {
            const playlistCard = document.createElement('div');
            playlistCard.className = 'playlist-card';
            playlistCard.innerHTML = `
                <div class="playlist-image">
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

    // Mostrar canciones de una playlist
    function showPlaylistSongs(playlistId, playlistName) {
        currentPlaylistTitle.textContent = playlistName;
        playlistSongsContainer.innerHTML = '';
        
        const songs = mockData.playlistSongs[playlistId] || [];
        
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
                    <i class="fas fa-play"></i>
                    <i class="fas fa-trash-alt remove-song"></i>
                </div>
            `;
            playlistSongsContainer.appendChild(songElement);
        });
        
        document.querySelector('.playlists-section').classList.add('hidden');
        songsSection.classList.remove('hidden');
    }

    // Volver a la vista de playlists
    backToPlaylists.addEventListener('click', () => {
        songsSection.classList.add('hidden');
        document.querySelector('.playlists-section').classList.remove('hidden');
    });

    // Modal para nueva playlist
    newPlaylistBtn.addEventListener('click', () => {
        newPlaylistModal.classList.remove('hidden');
    });

    closeModal.addEventListener('click', () => {
        newPlaylistModal.classList.add('hidden');
    });

    createPlaylistBtn.addEventListener('click', () => {
        const playlistName = playlistNameInput.value.trim();
        if (playlistName) {
            // Aquí iría la lógica para crear la nueva playlist
            alert(`Playlist "${playlistName}" creada`);
            playlistNameInput.value = '';
            newPlaylistModal.classList.add('hidden');
            // Recargar playlists
            loadPlaylists();
        }
    });

    // Cerrar modal al hacer clic fuera
    window.addEventListener('click', (e) => {
        if (e.target === newPlaylistModal) {
            newPlaylistModal.classList.add('hidden');
        }
    });

    // Inicializar
    loadPlaylists();
});