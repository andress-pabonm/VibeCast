const MusicPlayer = (function () {
  let currentAudio = null;
  let isPlaying = false;
  let currentQueue = [];
  let currentIndex = 0;
  let volume = 0.8;

  // Elementos del DOM
  const $ = (id) => document.getElementById(id);
  const playPauseBtn = $("playPauseBtn");
  const prevBtn = $("prevBtn");
  const nextBtn = $("nextBtn");
  const progressBar = $("progressBar");
  const progress = $("progress");
  const currentTimeEl = $("currentTime");
  const totalTimeEl = $("totalTime");
  const volumeSlider = $("volumeSlider");
  const volumeLevel = $("volumeLevel");
  const currentSongTitle = $("currentSongTitle");
  const currentSongArtist = $("currentSongArtist");
  const showQueueBtn = $("showQueueBtn");
  const closePlayerBtn = $("closePlayerBtn");
  const musicPlayer = $("musicPlayer");

  function formatTime(seconds) {
    const minutes = Math.floor(seconds / 60);
    const secs = Math.floor(seconds % 60);
    return `${minutes}:${secs < 10 ? "0" : ""}${secs}`;
  }

  function cleanupAudio() {
    if (currentAudio) {
      currentAudio.pause();
      currentAudio.src = ""; // Liberar
      currentAudio = null;
    }
  }

  function playSong() {
    // Pide la siguiente canción de la cola al backend
    window
      .dequeue()
      .then((res) => {
        if (res?.status !== "ok" || !res.data || !res.data.youtubeId) {
          console.warn(
            "No se pudo reproducir la siguiente canción:",
            res?.message || res
          );
          return;
        }

        const song = res.data; // debe contener: title, artist, youtubeId

        // Limpiar cualquier audio o iframe anterior
        cleanupAudio();

        // Crear el reproductor de YouTube
        createYouTubePlayer(song.youtubeId, () => {
          showNotification(`🎵 Canción terminada: ${song.title}`);
          playNext(); // seguir con la siguiente canción
        });

        // Actualizar UI
        currentSongTitle.textContent = song.title || "Sin título";
        currentSongArtist.textContent = song.artist || "Desconocido";
        isPlaying = true;
        playPauseBtn.innerHTML = '<i class="fas fa-pause"></i>';

        // Actualizar índice si está en la cola local
        currentIndex = currentQueue.findIndex((s) => s.id === song.id);
      })
      .catch((err) => {
        console.error("Error al hacer dequeue:", err);
      });
  }

  function showNotification(msg) {
    alert(msg);
  }

  function playNext() {
    playSong(); // usa dequeue() para la próxima canción
  }

  function togglePlayer() {
    musicPlayer.classList.toggle("hidden");
    document.body.classList.toggle("player-closed");
  }

  function init() {
    playPauseBtn.addEventListener("click", () => {
      if (!currentAudio) {
        if (currentQueue.length > 0) {
          playSong(currentQueue[0]);
        }
        return;
      }

      if (isPlaying) {
        currentAudio.pause();
        playPauseBtn.innerHTML = '<i class="fas fa-play"></i>';
        isPlaying = false;
      } else {
        currentAudio
          .play()
          .then(() => {
            playPauseBtn.innerHTML = '<i class="fas fa-pause"></i>';
            isPlaying = true;
          })
          .catch(console.error);
      }
    });

    prevBtn.addEventListener("click", () => {
      if (currentQueue.length > 0 && currentIndex > 0) {
        playSong(currentQueue[currentIndex - 1]);
      }
    });

    nextBtn.addEventListener("click", playNext);

    progressBar.addEventListener("click", (e) => {
      if (!currentAudio || !currentAudio.duration) return;
      const rect = progressBar.getBoundingClientRect();
      const percent = (e.clientX - rect.left) / rect.width;
      currentAudio.currentTime = percent * currentAudio.duration;
    });

    volumeSlider.addEventListener("click", (e) => {
      const rect = volumeSlider.getBoundingClientRect();
      const percent = (e.clientX - rect.left) / rect.width;
      volume = Math.min(1, Math.max(0, percent));
      volumeLevel.style.width = `${volume * 100}%`;
      if (currentAudio) currentAudio.volume = volume;
    });

    showQueueBtn.addEventListener("click", () => {
      if (currentQueue.length > 0) {
        alert(
          `Cola:\n${currentQueue
            .map((s, i) => `${i + 1}. ${s.title} - ${s.artist}`)
            .join("\n")}`
        );
      } else {
        alert("La cola de reproducción está vacía");
      }
    });

    closePlayerBtn.addEventListener("click", togglePlayer);

    volumeLevel.style.width = `${volume * 100}%`;
  }

  return {
    init,
    playSong,
    addSongsToQueue(songs, autoPlay = true) {
      if (!Array.isArray(songs)) songs = [songs];

      songs.forEach((song, i) => {
        if (!song || !song.id) {
          console.warn("Canción inválida:", song);
          return;
        }

        window
          .enqueue(song.id)
          .then((res) => {
            if (res?.status === "ok") {
              console.log(`✅ Encolada: ${song.title}`);
              currentQueue.push(song); // mantener cola local para mostrar

              // Reproducir si es la primera y no hay nada sonando
              if (autoPlay && !isPlaying && i === 0) {
                playSong(); // llama dequeue() y reproduce la próxima canción
              }
            } else {
              console.warn(
                `❌ No se pudo encolar: ${song.title}`,
                res?.message || res
              );
            }
          })
          .catch((err) => {
            console.error(`Error al encolar ${song.title}:`, err);
          });
      });
    },
    getCurrentQueue() {
      return currentQueue;
    },
  };
})();

function loadYouTubeAPI() {
  if (window.YT) return;
  const tag = document.createElement("script");
  tag.src = "https://www.youtube.com/iframe_api";
  document.head.appendChild(tag);
}

let youtubePlayer;

function createYouTubePlayer(videoId, onEndCallback) {
  // Elimina el iframe anterior si existe
  const existing = document.getElementById("youtube-player");
  if (existing) existing.remove();

  const iframe = document.createElement("div");
  iframe.id = "youtube-player";
  document.body.appendChild(iframe); // puedes ocultarlo luego

  youtubePlayer = new YT.Player("youtube-player", {
    videoId: videoId,
    events: {
      onReady: (e) => e.target.playVideo(),
      onStateChange: (e) => {
        if (e.data === YT.PlayerState.ENDED && onEndCallback) {
          onEndCallback();
        }
      },
    },
  });
}
