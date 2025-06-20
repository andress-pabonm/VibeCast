document.addEventListener('DOMContentLoaded', () => {
    /* AQUI IRIA TODA LA LOGICA PARA CARGAR LOS DATOS DEL SQLLITE */
    
    
    
    // Simulación de datos
    const mockData = {
        songs: [
            { id: 1, title: "Bohemian Rhapsody", artist: "Queen", duration: "5:55" },
            { id: 2, title: "Imagine", artist: "John Lennon", duration: "3:04" },
            { id: 3, title: "Billie Jean", artist: "Michael Jackson", duration: "4:54" },
            { id: 4, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56" },
            { id: 5, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56" },
            { id: 6, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56" },
            { id: 7, title: "Sweet Child O'Mine", artist: "Guns N' Roses", duration: "5:56" },
        ],
        friends: [
            { id: 1, name: "María García" },
            { id: 2, name: "Carlos López" },
            { id: 3, name: "Ana Martínez" },
            { id: 4, name: "Rocio xd"},
            { id: 5, name: "Pableis"}
        ],
        recommendations: [
            { id: 1, title: "Dancing Queen", artist: "ABBA", friend: "María García", duration: "3:50" },
            { id: 2, title: "Hotel California", artist: "Eagles", friend: "Carlos López", duration: "6:30" }
        ]
    };

    // Cargar canciones
    const songsContainer = document.getElementById('songs-container');
    mockData.songs.forEach(song => {
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
                <i class="fas fa-plus"></i>
            </div>
        `;
        songsContainer.appendChild(songElement);
    });

    // Cargar amigos
    const friendsContainer = document.getElementById('friends-container');
    mockData.friends.forEach(friend => {
        const friendElement = document.createElement('div');
        friendElement.className = 'friend-item';
        friendElement.innerHTML = `
            <div class="friend-avatar">
                <i class="fas fa-user"></i>
            </div>
            <div class="friend-name">${friend.name}</div>
            <i class="fas fa-chevron-right"></i>
        `;
        friendsContainer.appendChild(friendElement);
    });

    // Cargar recomendaciones
    const recommendationsContainer = document.getElementById('recommendations-container');
    mockData.recommendations.forEach(song => {
        const songElement = document.createElement('div');
        songElement.className = 'song-item';
        songElement.innerHTML = `
            <div class="song-info">
                <strong>${song.title}</strong>
                <p>${song.artist} · Recomendado por ${song.friend}</p>
            </div>
            <div class="song-actions">
                <span>${song.duration}</span>
                <i class="fas fa-play"></i>
            </div>
        `;
        recommendationsContainer.appendChild(songElement);
    });

    // Evento para el botón de perfil
    const profileBtn = document.querySelector('.profile-btn');
    if (profileBtn) {
        profileBtn.addEventListener('click', () => {
            window.location.href = '../Perfil/perfil.html';
        });
    }

    // Eventos para las canciones
    document.querySelectorAll('.song-item').forEach(item => {
        item.addEventListener('click', (e) => {
            if (!e.target.classList.contains('fa-play') && !e.target.classList.contains('fa-plus')) {
                const songTitle = item.querySelector('strong').textContent;
                console.log(`Reproduciendo: ${songTitle}`);
                // Aquí iría la lógica para reproducir la canción
            }
        });
    });
});