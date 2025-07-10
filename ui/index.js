// Para obtener el primer elemento que coincide con un selector
const getElement = (selector) => document.querySelector(selector);

// Para obtener todos los elementos que coinciden con un selector
const getElements = (selector) => document.querySelectorAll(selector);

document.addEventListener("DOMContentLoaded", () => {
  // Inicializa el reproductor
  MusicPlayer.init();

  // Secciones
  const sections = ["inicio", "biblioteca", "historial", "amigos", "perfil"];

  // Botones del menú
  const buttons = Object.fromEntries(
    sections.map((name) => [name, getElement(`#btn-${name}`)])
  );

  // Contenedor dinámico
  const dynamicContent = getElement(".main-content");

  // Cargar página dinámica
  async function loadPage(page) {
    try {
      const response = await fetch(`pages/${page}.html`);
      const html = await response.text();

      // Crea un contenedor temporal para procesar el HTML
      const tempDiv = document.createElement('div');
      tempDiv.innerHTML = html;

      // Extrae y aplica los estilos primero
      const styles = tempDiv.querySelector('style');
      if (styles) document.head.appendChild(styles.cloneNode(true));

      // Inserta el contenido en el DOM
      document.getElementById('main-content').innerHTML = tempDiv.querySelector('main').innerHTML;

    } catch (error) {
      console.error("Error cargando la página:", error);
    }
  }

  // Cargar JS de página
  function loadPageScript(page) {
    const oldScript = document.getElementById("page-script");
    if (oldScript) oldScript.remove();

    const script = document.createElement("script");
    script.id = "page-script";
    script.src = `js/${page}.js`;
    script.onerror = () => console.warn(`Script no encontrado: ${page}.js`);
    document.body.appendChild(script);
  }

  // Eventos de navegación
  sections.forEach((section) => {
    buttons[section].addEventListener("click", (e) => {
      e.preventDefault();
      loadPage(section);
      history.pushState({ page: section }, "", `#${section}`);
    });
  });

  // Manejar botón Atrás
  window.addEventListener("popstate", (e) => {
    const page = e.state?.page || "inicio";
    loadPage(page);
  });

  // Cargar página inicial
  const initialPage = window.location.hash.slice(1) || "inicio";
  loadPage(initialPage);
});




//REPRODUCTOR
// 2. This code loads the IFrame Player API code asynchronously.
var tag = document.createElement("script");

tag.src = "https://www.youtube.com/iframe_api";
var firstScriptTag = document.getElementsByTagName("script")[0];
firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);

// 3. This function creates an <iframe> (and YouTube player)
//    after the API code downloads.
// var player;
// function onYouTubeIframeAPIReady() {
//   player = new YT.Player("player", {
//     height: "390",
//     width: "640",
//     videoId: "PgBvV9ofjmA",
//     playerVars: {
//       playsinline: 1,
//     },
//     events: {
//       onReady: onPlayerReady,
//       onStateChange: onPlayerStateChange,
//     },
//   });
// }

// 4. The API will call this function when the video player is ready.
function onPlayerReady(event) {
  event.target.playVideo();
}

// 5. The API calls this function when the player's state changes.
//    The function indicates that when playing a video (state=1),
//    the player should play for six seconds and then stop.
var done = false;
function onPlayerStateChange(event) {
  if (event.data == YT.PlayerState.PLAYING && !done) {
    setTimeout(stopVideo, 6000);
    done = true;
  }
}
function stopVideo() {
  player.stopVideo();
}

const MusicPlayer = (function () {
  let currentAudio = null;
  let isPlaying = false;
  let currentQueue = [];
  let currentIndex = 0;
  let volume = 1;

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

  function loadYouTubeAPI() {
    const tag = document.createElement('script');
    tag.src = "https://www.youtube.com/iframe_api";
    document.head.appendChild(tag);
  }

  function createYouTubePlayer(videoId, onEndCallback) {
    new YT.Player("player", {
      videoId: videoId,
      events: {
        'onStateChange': (event) => {
          if (event.data === YT.PlayerState.ENDED) onEndCallback();
        }
      }
    });
  }

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
    loadYouTubeAPI();

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
    addSongsToQueue(songs, autoPlay) {
      loadYouTubeAPI();
      if (!Array.isArray(songs)) songs = [songs];

      if (autoPlay) {
        // Vaciar cola de reproducción antes de añadir nuevas canciones
        window
          .vaciar_cola()
          .then(() => {
            console.log("Cola vaciada para reproducción automática");
          })
          .catch((err) => {
            console.error("Error al vaciar la cola:", err);
          });
      }

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
              if (!isPlaying) {
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
  };
})();
