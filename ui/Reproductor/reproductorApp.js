//TODA LA LOGICA DE REPRODUCCION DE MUSICA

// Reproductor de música como módulo
const MusicPlayer = (function() {
    // Variables privadas
    let currentAudio = new Audio();
    let isPlaying = false;
    let currentQueue = [];
    let currentIndex = 0;
    let volume = 0.8;
    
    // Elementos del DOM
    const playPauseBtn = document.getElementById('playPauseBtn');
    const prevBtn = document.getElementById('prevBtn');
    const nextBtn = document.getElementById('nextBtn');
    const progressBar = document.getElementById('progressBar');
    const progress = document.getElementById('progress');
    const currentTimeEl = document.getElementById('currentTime');
    const totalTimeEl = document.getElementById('totalTime');
    const volumeSlider = document.getElementById('volumeSlider');
    const volumeLevel = document.getElementById('volumeLevel');
    const currentSongTitle = document.getElementById('currentSongTitle');
    const currentSongArtist = document.getElementById('currentSongArtist');
    const showQueueBtn = document.getElementById('showQueueBtn');
    const closePlayerBtn = document.getElementById('closePlayerBtn');
    const musicPlayer = document.getElementById('musicPlayer');
    
    // Formatear tiempo (segundos a mm:ss)
    function formatTime(seconds) {
        const minutes = Math.floor(seconds / 60);
        const secs = Math.floor(seconds % 60);
        return `${minutes}:${secs < 10 ? '0' : ''}${secs}`;
    }
    
    // Función para reproducir una canción
    function playSong(song) {
        if (isPlaying) {
            currentAudio.pause();
        }
        
        currentAudio = new Audio(`../../assets/music/${idol.mp3}`);
        currentAudio.volume = volume;
        
        currentAudio.addEventListener('loadedmetadata', () => {
            totalTimeEl.textContent = formatTime(currentAudio.duration);
        });
        
        currentAudio.addEventListener('timeupdate', () => {
            const percent = (currentAudio.currentTime / currentAudio.duration) * 100;
            progress.style.width = `${percent}%`;
            currentTimeEl.textContent = formatTime(currentAudio.currentTime);
        });
        
        //PUES AQUI ESTA SI SE TERMINA LA CANCION
        currentAudio.addEventListener('ended', () => {
            // Mostrar notificación
            showNotification(`Canción terminada: ${currentQueue[currentIndex].title}`);
            playNext();
        });
        
        //MUESTRA EL MENSAJE Q SE TERMINO XD
        function showNotification(message) {
            alert(message);
        }
        
        currentAudio.play();
        isPlaying = true;
        playPauseBtn.innerHTML = '<i class="fas fa-pause"></i>';
        
        currentSongTitle.textContent = song.title;
        currentSongArtist.textContent = song.artist;
        
        // Actualizar el índice actual si está en la cola
        currentIndex = currentQueue.findIndex(s => s.id === song.id);
    }
    
    // Función para canción siguiente
    function playNext() {
        if (currentQueue.length > 0 && currentIndex < currentQueue.length - 1) {
            currentIndex++;
            playSong(currentQueue[currentIndex]);
        }
    }
    
    // Inicializar eventos
    function init() {
        // Control de reproducción/pausa
        playPauseBtn.addEventListener('click', () => {
            if (isPlaying) {
                currentAudio.pause();
                playPauseBtn.innerHTML = '<i class="fas fa-play"></i>';
            } else {
                if (currentAudio.src) {
                    currentAudio.play();
                    playPauseBtn.innerHTML = '<i class="fas fa-pause"></i>';
                } else if (currentQueue.length > 0) {
                    playSong(currentQueue[0]);
                }
            }
            isPlaying = !isPlaying;
        });
        
        // Canción anterior
        prevBtn.addEventListener('click', () => {
            if (currentQueue.length > 0 && currentIndex > 0) {
                currentIndex--;
                playSong(currentQueue[currentIndex]);
            }
        });
        
        // Canción siguiente
        nextBtn.addEventListener('click', () => {
            playNext();
        });
        
        // Control de volumen
        volumeSlider.addEventListener('click', (e) => {
            const rect = volumeSlider.getBoundingClientRect();
            const percent = (e.clientX - rect.left) / rect.width;
            volume = Math.max(0, Math.min(1, percent));
            volumeLevel.style.width = `${volume * 100}%`;
            if (currentAudio) currentAudio.volume = volume;
        });
        
        // Control de progresoz
        progressBar.addEventListener('click', (e) => {
            if (!currentAudio.src) return;
            const rect = progressBar.getBoundingClientRect();
            const percent = (e.clientX - rect.left) / rect.width;
            currentAudio.currentTime = percent * currentAudio.duration;
        });
        
        // Ver cola de reproducción
        showQueueBtn.addEventListener('click', () => {
            if (currentQueue.length > 0) {
                alert(`Canciones en cola:\n${currentQueue.map((song, i) => `${i+1}. ${song.title} - ${song.artist}`).join('\n')}`);
            } else {
                alert('La cola de reproducción está vacía');
            }
        });
        
        // Inicializar volumen
        volumeLevel.style.width = `${volume * 100}%`;

        closePlayerBtn.addEventListener('click', togglePlayer);
    }
    
    // API pública
    return {
        init,
        playSong,
        addSongsToQueue: function(songs) {
            currentQueue = currentQueue.concat(songs);
            if (!isPlaying) {
                playSong(currentQueue[0]);
            }
        },
        getCurrentQueue: function() {
            return currentQueue;
        }
    };

    //NO SE CIERRA XDDD ya no se que hago no se mucho de js jaja
    // Función para cerrar el reproductor
    function togglePlayer() {
        musicPlayer.classList.toggle('hidden');
        document.body.classList.toggle('player-closed');
    }
});

// Inicializar el reproductor cuando el DOM esté listo
document.addEventListener('DOMContentLoaded', () => {
    MusicPlayer.init();
});