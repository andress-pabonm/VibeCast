const MusicPlayer = (function () {
  const $ = (sel) => document.querySelector(sel);
  const $$ = (id) => document.getElementById(id);

  // Cargar la API de YouTube
  const tag = document.createElement("script");
  tag.src = "https://www.youtube.com/iframe_api";
  // document.getElementsByTagName("script")[0].parentNode.insertBefore(tag, tag);
  document.head.appendChild(tag);

  let player;
  let isPlaying = false;
  let isPaused = false;
  let isDraggingVolume = false;
  let isDraggingProgress = false;
  let updateInterval;

  function init() {
    togglePlayerVisibility(false);

    player = new YT.Player("player", {
      playerVars: { controls: 0, fs: 0 },
      events: {
        onReady: onPlayerReady,
        onStateChange: onPlayerStateChange,
      },
    });
  }

  function togglePlayerVisibility(show = true) {
    document.body.classList.toggle("player-closed", !show);
    $(`.music-player`)?.classList.toggle("hidden", !show);
  }

  function onPlayerReady(event) {
    setupPlayerControls();
    event.target.setVolume(100);
    updateVolumeDisplay(100);
    playSong();
  }

  function onPlayerStateChange(event) {
    const playBtn = $$("playPauseBtn");

    switch (event.data) {
      case YT.PlayerState.PLAYING:
        isPlaying = true;
        isPaused = false;
        playBtn.innerHTML = '<i class="fas fa-pause"></i>';
        startProgressUpdate();
        togglePlayerVisibility(true);
        break;

      case YT.PlayerState.PAUSED:
        isPlaying = false;
        isPaused = true;
        playBtn.innerHTML = '<i class="fas fa-play"></i>';
        stopProgressUpdate();
        break;

      case YT.PlayerState.ENDED:
        if (!isPlaying) break;

        isPlaying = false;
        isPaused = false;
        playBtn.innerHTML = '<i class="fas fa-play"></i>';
        stopProgressUpdate();
        togglePlayerVisibility(false);

        playSong();
        break;
    }
  }

  function setupPlayerControls() {
    $$("playPauseBtn").addEventListener("click", togglePlayPause);
    $$("nextBtn").addEventListener("click", playSong);
    $$("prevBtn").addEventListener("click", showQueueAlert);
    $$("closePlayerBtn").addEventListener("click", () => {
      player.pauseVideo();
      togglePlayerVisibility(false);
    });

    const volumeSlider = $$("volumeSlider");
    volumeSlider.addEventListener("mousedown", startVolumeDrag);
    volumeSlider.addEventListener("click", handleVolumeClick);
    document.addEventListener("mousemove", handleVolumeDrag);
    document.addEventListener("mouseup", stopVolumeDrag);

    const progressBar = $$("progressBar");
    progressBar.addEventListener("mousedown", startProgressDrag);
    progressBar.addEventListener("click", handleProgressClick);
    document.addEventListener("mousemove", handleProgressDrag);
    document.addEventListener("mouseup", stopProgressDrag);
  }

  function togglePlayPause() {
    isPlaying ? player.pauseVideo() : player.playVideo();
  }

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
    const slider = $$("volumeSlider");
    const rect = slider.getBoundingClientRect();
    let volume = Math.round(((e.clientX - rect.left) / rect.width) * 100);
    volume = Math.max(0, Math.min(100, volume));
    player.setVolume(volume);
    updateVolumeDisplay(volume);
  }
  function updateVolumeDisplay(volume) {
    $$("volumeLevel").style.width = volume + "%";
    const icon = $(".volume-control i");
    icon.className = getVolumeIcon(volume);
  }
  function getVolumeIcon(volume) {
    if (volume === 0) return "fas fa-volume-mute";
    if (volume < 50) return "fas fa-volume-down";
    return "fas fa-volume-up";
  }

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
    const bar = $$("progressBar");
    const rect = bar.getBoundingClientRect();
    let percent = ((e.clientX - rect.left) / rect.width) * 100;
    percent = Math.max(0, Math.min(100, percent));
    setProgressWidth(percent);

    const duration = player.getDuration();
    const newTime = (percent / 100) * duration;
    player.seekTo(newTime, true);
    updateTimeInfo(newTime, duration);
  }

  function setProgressWidth(percent) {
    $$("progress").style.width = `${percent}%`;
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
    if (!isDraggingProgress) setProgressWidth(percent);
    updateTimeInfo(currentTime, duration);
  }
  function updateTimeInfo(currentTime, duration) {
    $$("currentTime").textContent = formatTime(currentTime);
    if (duration) $$("totalTime").textContent = formatTime(duration);
  }
  function updateSongInfo(title, artist) {
    $$("currentSongTitle").textContent = title || "No hay canción seleccionada";
    $$("currentSongArtist").textContent = artist || "VibeCast";
  }

  async function safeCall(fn, onError = console.warn) {
    try {
      return await fn();
    } catch (err) {
      onError(err?.message || err);
    }
  }

  async function playSong() {
    if (!player?.loadVideoById)
      return console.warn("Reproductor no disponible.");

    player.stopVideo();
    player.loadVideoById("");

    const res = await safeCall(() => window.decolar());
    if (res?.status !== "ok") {
      togglePlayerVisibility(false);
      return;
    }

    const song = res.data;
    if (!song) return;

    togglePlayerVisibility(true);
    player.loadVideoById(song.url);
    updateSongInfo(song.title, song.artist);
  }

  async function addSongToQueue(song, reset = false) {
    if (!player || !song) return;
    if (reset) await emptyQueue();

    const res = await safeCall(() => window.encolar(song.id));
    if (res?.status !== "ok") return;

    if (reset) playSong();
    else if (isPaused) player.playVideo();
    else if (!isPlaying) playSong;
  }

  async function emptyQueue() {
    const res = await safeCall(() => window.vaciar_cola());
    if (res?.status !== "ok") return;
    console.log(res?.message || "Cola de reproducción vaciada.");
  }

  function addSongsToQueue(songs) {
    if (!Array.isArray(songs)) songs = [songs];
    songs.forEach(async (song) => await addSongToQueue(song));
  }

  function showQueueAlert() {
    alert(
      "En una cola de reproducción no se puede retroceder\nLas canciones solo avanzan en orden de llegada"
    );
  }

  return {
    init,
    addSongToQueue,
    addSongsToQueue,
  };
})();

function onYouTubeIframeAPIReady() {
  console.log("YouTube Iframe API ready!");
  MusicPlayer.init();
}
