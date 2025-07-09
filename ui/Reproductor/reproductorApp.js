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

  function playSong(song) {
    if (!song || !song.title || !song.id) {
      console.warn("Canción inválida", song);
      return;
    }

    cleanupAudio();

    const audio = new Audio(song.path || "../../assets/music/Idol.mp3");
    audio.volume = volume;

    audio.addEventListener("loadedmetadata", () => {
      totalTimeEl.textContent = formatTime(audio.duration);
    });

    audio.addEventListener("timeupdate", () => {
      const percent = (audio.currentTime / audio.duration) * 100;
      progress.style.width = `${percent}%`;
      currentTimeEl.textContent = formatTime(audio.currentTime);
    });

    audio.addEventListener("ended", () => {
      showNotification(`Canción terminada: ${song.title}`);
      playNext();
    });

    audio
      .play()
      .then(() => {
        isPlaying = true;
        currentAudio = audio;
        playPauseBtn.innerHTML = '<i class="fas fa-pause"></i>';
        currentSongTitle.textContent = song.title;
        currentSongArtist.textContent = song.artist || "Desconocido";
        currentIndex = currentQueue.findIndex((s) => s.id === song.id);
      })
      .catch((err) => {
        console.error("No se pudo reproducir:", err);
        showNotification("Error al reproducir la canción.");
      });
  }

  function showNotification(msg) {
    alert(msg);
  }

  function playNext() {
    if (currentQueue.length === 0) return;
    const next = currentQueue[currentIndex + 1];
    if (next) {
      playSong(next);
    } else {
      console.log("Fin de la cola");
      isPlaying = false;
      playPauseBtn.innerHTML = '<i class="fas fa-play"></i>';
    }
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
    addSongsToQueue(songs) {
      if (!Array.isArray(songs)) return;
      currentQueue = currentQueue.concat(songs);
      if (!isPlaying) {
        playSong(currentQueue[0]);
      }
    },
    getCurrentQueue() {
      return currentQueue;
    },
  };
})();
