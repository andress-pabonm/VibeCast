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
            { id: 5, title: "AYAYAYA", artist: "SI", duration: "2:00" }
        ],
        recommendations: [
            { id: 1, title: "Dancing Queen", artist: "ABBA", friend: "María García", duration: "3:50" },
            { id: 2, title: "Hotel California", artist: "Eagles", friend: "Carlos López", duration: "6:30" },
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

    // Evento para el botón de cerrar sesión ANDRES AQUI IMPLEMENTA BIEN LA LOGICA XDXD
    const logoutBtn = document.querySelector('.logout-btn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', () => {
            // Aquí iría la lógica para cerrar sesión
            // Por ejemplo:
            fetch('/logout', { method: 'POST' })
                .then(() => {
                    window.location.href = '../Login/login.html';
                })
                .catch(error => {
                    console.error('Error al cerrar sesión:', error);
                });
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