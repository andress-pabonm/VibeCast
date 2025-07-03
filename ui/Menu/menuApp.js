document.addEventListener("DOMContentLoaded", () => {
  checkUserLoggedIn();

  loadSongs();
  loadRecommendations();

  setupSongClickEvents();
});

/**
 * Verifica si hay sesión iniciada.
 */
async function checkUserLoggedIn() {
  try {
    const res = await window.is_logged_in();
    console.log("is_logged_in():", res);

    if (res.status !== "ok" || res.type !== "boolean" || res.data !== true) {
      window.location.replace("../Login/index.html");
    }
  } catch (err) {
    console.error("Error al verificar sesión:", err);
  }
}

/**
 * Carga las canciones disponibles desde el backend.
 */
async function loadSongs() {
  const songsContainer = document.getElementById("songs-container");
  if (!songsContainer) return;

  try {
    const res = await window.get_canciones(); // ← reemplaza mock
    console.log("canciones():", res);

    if (
      res.status === "ok" &&
      res.type === "array" &&
      Array.isArray(res.data)
    ) {
      res.data.forEach((song) => {
        const songElement = createSongElement(song);
        songsContainer.appendChild(songElement);
      });
    } else {
      console.warn(
        "No se pudo cargar canciones:",
        res.message || "Respuesta inválida."
      );
    }
  } catch (err) {
    console.error("Error al cargar canciones:", err);
  }
}

/**
 * Carga las canciones recomendadas desde el backend.
 */
async function loadRecommendations() {
  const recommendationsContainer = document.getElementById(
    "recommendations-container"
  );
  if (!recommendationsContainer) return;

  try {
    const res = await window.get_recomendaciones(); // ← reemplaza mock
    console.log("recomendaciones():", res);

    if (
      res.status === "ok" &&
      res.type === "array" &&
      Array.isArray(res.data)
    ) {
      res.data.forEach((song) => {
        const songElement = createRecommendationElement(song);
        recommendationsContainer.appendChild(songElement);
      });
    } else {
      console.warn(
        "No se pudo cargar recomendaciones:",
        res.message || "Respuesta inválida."
      );
    }
  } catch (err) {
    console.error("Error al cargar recomendaciones:", err);
  }
}

/**
 * Crea un elemento DOM para una canción.
 */
function createSongElement(song) {
  const div = document.createElement("div");
  div.className = "song-item";
  div.innerHTML = `
    <div class="song-info">
        <strong>${song.title}</strong>
        <p>${song.artist}</p>
    </div>
    <div class="song-actions">
        <span>${song.duration}</span>
        <i class="fas fa-play"></i>
        <i class="fas fa-plus"></i>
    </div>`;
  return div;
}

/**
 * Crea un elemento DOM para una canción recomendada.
 */
function createRecommendationElement(song) {
  const div = document.createElement("div");
  div.className = "song-item";
  div.innerHTML = `
    <div class="song-info">
        <strong>${song.title}</strong>
        <p>${song.artist} · Recomendado por ${song.friend}</p>
    </div>
    <div class="song-actions">
        <span>${song.duration}</span>
        <i class="fas fa-play"></i>
    </div>`;
  return div;
}

/**
 * Configura eventos para manejar clics en canciones.
 */
function setupSongClickEvents() {
  document.addEventListener("click", (e) => {
    const item = e.target.closest(".song-item");
    if (!item) return;

    // Ignorar si se hizo clic en los íconos
    if (
      e.target.classList.contains("fa-play") ||
      e.target.classList.contains("fa-plus")
    ) {
      return;
    }

    const title = item.querySelector("strong")?.textContent;
    if (title) {
      console.log(`Reproduciendo: ${title}`);
      // Aquí iría la lógica para:
      // - añadir a la cola: window.add_to_queue(id);
      // - o reproducir directamente
    }
  });
}
