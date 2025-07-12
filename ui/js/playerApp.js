const MusicPlayer = (function () {
  // Cargar la API de YouTube
  var tag = document.createElement("script");
  tag.src = "https://www.youtube.com/iframe_api";
  var firstScriptTag = document.getElementsByTagName("script")[0];
  firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);

  let player;
  let isPlaying = false;
  let isDraggingVolume = false;
  let isDraggingProgress = false;
  let updateInterval;

  function init(song) {
    player = new YT.Player("player", {
      playerVars: {
        controls: 0,
        disablekb: 1,
        fs: 0,
      },
      events: {
        onReady: onPlayerReady,
        onStateChange: onPlayerStateChange,
      },
    });

    setupPlayerControls();
    document.body.classList.add("player-closed");
    document.querySelector(".music-player").classList.add("hidden");
  }

  function onPlayerReady(event) {
    // Configuración inicial
    event.target.setVolume(100); // Volumen inicial al 100%
    updateVolumeDisplay(100);
    playSong();
  }

  function onPlayerStateChange(event) {
    if (event.data == YT.PlayerState.PLAYING) {
      isPlaying = true;
      document.getElementById("playPauseBtn").innerHTML =
        '<i class="fas fa-pause"></i>';
      startProgressUpdate();
    } else if (event.data == YT.PlayerState.PAUSED) {
      isPlaying = false;
      document.getElementById("playPauseBtn").innerHTML =
        '<i class="fas fa-play"></i>';
    } else if (event.data == YT.PlayerState.ENDED) {
      isPlaying = false;
      document.getElementById("playPauseBtn").innerHTML =
        '<i class="fas fa-play"></i>';
      stopProgressUpdate();
      playSong();
    }
  }

  function setupPlayerControls() {
    // Botón play/pause
    document
      .getElementById("playPauseBtn")
      .addEventListener("click", togglePlayPause);

    // Botón cerrar reproductor
    document
      .getElementById("closePlayerBtn")
      .addEventListener("click", function () {
        player.pauseVideo();
        document.body.classList.add("player-closed");
        document.querySelector(".music-player").classList.add("hidden");
      });

    // Control de volumen
    const volumeSlider = document.getElementById("volumeSlider");
    volumeSlider.addEventListener("mousedown", startVolumeDrag);
    volumeSlider.addEventListener("click", handleVolumeClick);

    document.addEventListener("mousemove", handleVolumeDrag);
    document.addEventListener("mouseup", stopVolumeDrag);

    // Barra de progreso
    const progressBar = document.getElementById("progressBar");
    progressBar.addEventListener("mousedown", startProgressDrag);
    progressBar.addEventListener("click", handleProgressClick);

    document.addEventListener("mousemove", handleProgressDrag);
    document.addEventListener("mouseup", stopProgressDrag);
  }

  function togglePlayPause() {
    if (isPlaying) {
      player.pauseVideo();
    } else {
      player.playVideo();
    }
  }

  // Funciones de volumen
  function startVolumeDrag(e) {
    isDraggingVolume = true;
    updateVolume(e);
  }

  function handleVolumeDrag(e) {
    if (isDraggingVolume) updateVolume(e);
  }

  function stopVolumeDrag() {
    isDraggingVolume = false;
  }

  function handleVolumeClick(e) {
    updateVolume(e);
  }

  function updateVolume(e) {
    const volumeSlider = document.getElementById("volumeSlider");
    const rect = volumeSlider.getBoundingClientRect();
    let volume = Math.round(((e.clientX - rect.left) / rect.width) * 100);

    volume = Math.max(0, Math.min(100, volume));
    player.setVolume(volume);
    updateVolumeDisplay(volume);
  }

  function updateVolumeDisplay(volume) {
    document.getElementById("volumeLevel").style.width = volume + "%";
    const volumeIcon = document.querySelector(".volume-control i");
    volumeIcon.className =
      volume === 0
        ? "fas fa-volume-mute"
        : volume < 50
        ? "fas fa-volume-down"
        : "fas fa-volume-up";
  }

  // Funciones de progreso
  function startProgressDrag(e) {
    isDraggingProgress = true;
    updateProgress(e);
    stopProgressUpdate();
  }

  function handleProgressDrag(e) {
    if (isDraggingProgress) updateProgress(e);
  }

  function stopProgressDrag() {
    isDraggingProgress = false;
    if (isPlaying) startProgressUpdate();
  }

  function handleProgressClick(e) {
    updateProgress(e);
  }

  function updateProgress(e) {
    const progressBar = document.getElementById("progressBar");
    const rect = progressBar.getBoundingClientRect();
    let percent = ((e.clientX - rect.left) / rect.width) * 100;

    percent = Math.max(0, Math.min(100, percent));
    document.getElementById("progress").style.width = percent + "%";

    const duration = player.getDuration();
    const newTime = (percent / 100) * duration;
    player.seekTo(newTime, true);

    updateTimeInfo(newTime, duration);
  }

  function startProgressUpdate() {
    stopProgressUpdate();
    updateProgressBar();
    updateInterval = setInterval(updateProgressBar, 1000);
  }

  function stopProgressUpdate() {
    clearInterval(updateInterval);
  }

  function updateProgressBar() {
    const currentTime = player.getCurrentTime();
    const duration = player.getDuration();
    const percent = (currentTime / duration) * 100;

    if (!isDraggingProgress) {
      document.getElementById("progress").style.width = percent + "%";
    }

    updateTimeInfo(currentTime, duration);
  }

  function updateTimeInfo(currentTime, duration) {
    document.getElementById("currentTime").textContent =
      formatTime(currentTime);
    if (duration) {
      document.getElementById("totalTime").textContent = formatTime(duration);
    }
  }

  function updateSongInfo(title, artist) {
    document.getElementById("currentSongTitle").textContent =
      title || "No hay canción seleccionada";
    document.getElementById("currentSongArtist").textContent =
      artist || "VibeCast";
  }

  async function playSong() {
    if (!player) {
      console.warn("Reproductor no disponible.");
      return;
    }

    try {
      const res = await window.decolar();
      console.log("decolar(): ", res);

      if (res.status !== "ok") {
        throw new Error(res.message);
      }

      console.log(res.message);
      const song = res.data;

      if (!song) return;

      if (player) {
        document.body.classList.remove("player-closed");
        document.querySelector(".music-player").classList.remove("hidden");
        player.loadVideoById(song.url);
        updateSongInfo(song.title, song.artist);
      }
    } catch (err) {
      console.log(err.message);
    }
  }

  async function addSongToQueue(song, reset = false) {
    if (!player || !song) return;

    if (reset) {
      try {
        const res = await window.vaciar_cola();
        console.log("vaciar_cola(): ", res);
        if (res.status !== "ok") {
          throw new Error(res.message);
        }

        player.stopVideo();
        isPlaying = false;
      } catch (err) {
        console.warn(
          "No fue posible vaciar la cola de reproducción. ",
          err.message
        );
      }
    }

    try {
      const res = await window.encolar(song.id);
      console.log("encolar(): ", res);

      if (res.status !== "ok") {
        throw new Error(res.message);
      }

      console.log(res.message);

      if (!isPlaying) {
        playSong();
      }
    } catch (err) {
      console.warn("Error al encolar canción. ", err.message);
    }
  }

  return {
    init,
    addSongToQueue,
  };
})();

function onYouTubeIframeAPIReady() {
  console.log("YouTube Iframe API ready!");
  MusicPlayer.init();
}
