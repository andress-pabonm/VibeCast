document.addEventListener('DOMContentLoaded', () => {
    //LOGICA DE HISTORIAL REAL
    
    // Datos de ejemplo del historial
    const mockHistory = [
        {
            id: 1,
            songId: 101,
            title: "Bohemian Rhapsody",
            artist: "Queen",
            duration: "5:55",
            playedAt: new Date(Date.now() - 1000 * 60 * 30), // 30 minutos atrás LOGICA DE TIEMPO
            plays: 3
        },
        {
            id: 2,
            songId: 102,
            title: "Imagine",
            artist: "John Lennon",
            duration: "3:04",
            playedAt: new Date(Date.now() - 1000 * 60 * 60 * 5), // 5 horas atrás
            plays: 1
        },
        {
            id: 3,
            songId: 103,
            title: "Sweet Child O'Mine",
            artist: "Guns N' Roses",
            duration: "5:56",
            playedAt: new Date(Date.now() - 1000 * 60 * 60 * 24), // 1 día atrás
            plays: 2
        },
        {
            id: 4,
            songId: 104,
            title: "Hotel California",
            artist: "Eagles",
            duration: "6:30",
            playedAt: new Date(Date.now() - 1000 * 60 * 60 * 24 * 3), // 3 días atrás
            plays: 1
        },
        {
            id: 5,
            songId: 105,
            title: "Blinding Lights",
            artist: "The Weeknd",
            duration: "3:20",
            playedAt: new Date(Date.now() - 1000 * 60 * 60 * 24 * 7), // 1 semana atrás
            plays: 5
        }
    ];

    // Elementos del DOM
    const historyContainer = document.getElementById('historyItemsContainer');

    // Formatear fecha
    function formatDate(date) {
        const now = new Date();
        const diffInHours = (now - date) / (1000 * 60 * 60);
        
        if (diffInHours < 24) {
            return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
        } else {
            return date.toLocaleDateString([], { day: 'numeric', month: 'short' });
        }
    }

    // Cargar historial
    function loadHistory(filter = 'all') {
        historyContainer.innerHTML = '';
        
        let filteredHistory = mockHistory;
        
        filteredHistory.sort((a, b) => b.playedAt - a.playedAt);
        
        filteredHistory.forEach(item => {
            const historyElement = document.createElement('div');
            historyElement.className = 'history-item';
            historyElement.innerHTML = `
                <div class="history-info">
                    <div class="song-title">${item.title}</div>
                    <div class="play-date">
                        <i class="far fa-clock"></i>
                        ${formatDate(item.playedAt)} · 
                        ${item.plays} ${item.plays === 1 ? 'reproducción' : 'reproducciones'}
                    </div>
                </div>
                <div class="history-actions">
                    <span>${item.duration}</span>
                    <i class="fas fa-play play-btn" title="Reproducir"></i>
                    <i class="fas fa-plus-circle add-to-playlist" title="Añadir a playlist"></i>
                </div>
            `; //AHI HAY UN ICONO PARA AGREGAR A UNA PLAYLIST, XQ CLARO PUEDE ESCUCHAR UNA CANCION PERO NO GUARDARLA
            //PERO QUE PASA SI YA ESAT GUARDADA EN UNA PLAYLIST??? COMO SE QUITARIA ESA OPCION, NO SE SI ME ENTIENDES
            historyContainer.appendChild(historyElement);
        });
    }

    // Inicializar
    loadHistory();
});