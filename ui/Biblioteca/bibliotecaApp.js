// Inicializar la aplicación cuando el DOM esté listo
document.addEventListener('DOMContentLoaded', () => {
    // Datos de ejemplo simulados
    const mockData = {
        playlists: [
            { id: 1, name: "Favoritas", songCount: 5 }, 
            { id: 2, name: "Rock Clásico", songCount: 3 },
            { id: 3, name: "Para estudiar", songCount: 4 },
            { id: 4, name: "Éxitos 2023", songCount: 7 },
            { id: 5, name: "Mixtape Verano", songCount: 6 }
        ],
        playlistSongs: {
            1: [
                { id: 1, title: "Bohemian Rhapsody", artist: "Queen", duration: "5:55", file: "bohemian.mp3" },
                { id: 2, title: "Imagine", artist: "John Lennon", duration: "3:04", file: "imagine.mp3" },
                { id: 3, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56", file: "sweet-child.mp3" },
                { id: 4, title: "Hotel California", artist: "Eagles", duration: "6:30", file: "hotel.mp3" },
                { id: 5, title: "Smells Like Teen Spirit", artist: "Nirvana", duration: "5:01", file: "teen-spirit.mp3" }
            ],
            2: [
                { id: 1, title: "Stairway to Heaven", artist: "Led Zeppelin", duration: "8:02", file: "stairway.mp3" },
                { id: 3, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56", file: "sweet-child.mp3" },
                { id: 5, title: "Smells Like Teen Spirit", artist: "Nirvana", duration: "5:01", file: "teen-spirit.mp3" }
            ],
            3: [
                { id: 6, title: "Clair de Lune", artist: "Claude Debussy", duration: "5:04", file: "clair-de-lune.mp3" },
                { id: 7, title: "River Flows In You", artist: "Yiruma", duration: "3:12", file: "river-flows.mp3" },
                { id: 8, title: "Moonlight Sonata", artist: "Beethoven", duration: "6:20", file: "moonlight.mp3" },
                { id: 9, title: "Gymnopédie No.1", artist: "Erik Satie", duration: "3:30", file: "gymnopedie.mp3" }
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
    const closeModal = document.querySelectorAll('.close-modal');
    const createPlaylistBtn = document.getElementById('createPlaylistBtn');
    const playlistNameInput = document.getElementById('playlistNameInput');
    const backToPlaylists = document.getElementById('backToPlaylists');
    const editPlaylistModal = document.getElementById('editPlaylistModal');
    const savePlaylistBtn = document.getElementById('savePlaylistBtn');
    const deletePlaylistBtn = document.getElementById('deletePlaylistBtn');
    const editPlaylistNameInput = document.getElementById('editPlaylistNameInput');
    const addToQueueBtn = document.getElementById('addToQueueBtn');
    
    // Variables globales
    let currentPlaylistId = null;
    
    // Cargar playlists
    function loadPlaylists() {
        playlistsContainer.innerHTML = '';
        mockData.playlists.forEach(playlist => {
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
        
        // Agregar eventos a los botones de opciones
        document.querySelectorAll('.playlist-options').forEach(btn => {
            btn.addEventListener('click', (e) => {
                e.stopPropagation();
                const playlistId = parseInt(btn.getAttribute('data-id'));
                editPlaylist(playlistId);
            });
        });
    }

    // Mostrar canciones de una playlist
    function showPlaylistSongs(playlistId, playlistName) {
        currentPlaylistId = playlistId;
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
                    <i class="fas fa-play play-btn" title="Reproducir"></i>
                </div>
            `;
            
            // Evento para reproducir la canción
            songElement.querySelector('.play-btn').addEventListener('click', (e) => {
                e.stopPropagation();
                MusicPlayer.playSong(song);
            });
            
            playlistSongsContainer.appendChild(songElement);
        });
        
        document.querySelector('.playlists-section').classList.add('hidden');
        songsSection.classList.remove('hidden');
    }

    // Editar playlist
    function editPlaylist(playlistId) {
        const playlist = mockData.playlists.find(p => p.id === playlistId);
        if (!playlist) return;
        
        currentPlaylistId = playlistId;
        editPlaylistNameInput.value = playlist.name;
        editPlaylistModal.classList.remove('hidden');
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

    // Cerrar modales
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
            loadPlaylists();
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

    // Inicializar la aplicación
    loadPlaylists();
});