window.views = window.views || {};

window.views.historial = {
  title: `<i class="fas fa-history"></i>Historial`,
  html: `
        <header class="main-header">
        <div class="search-bar">
            <i class="fas fa-search search-icon"></i>
            <input type="text" placeholder="Buscar en tu historial..." class="search-input">
        </div>
    </header> 

    <div class="history-container">
        <!-- Lista de historial -->
        <section class="history-list">
            <h2 class="section-title"><i class="fas fa-clock"></i> Recientemente escuchado</h2>
            <div class="song-list" id="historyItemsContainer">
                <!-- Historial se cargará dinámicamente -->
            </div>
        </section>
    </div>
  `,
  init: function () {
    // Elementos del DOM
    const historyContainer = document.getElementById("historyItemsContainer");

    // Cargar historial
    async function loadHistory() {
      historyContainer.innerHTML = "";

      try {
        const res = await window.obtener_historial();

        if (res.status !== "ok") {
          throw new Error(res.message);
        }

        console.log(res.message);

        let filteredHistory = res.data;

        filteredHistory.sort((a, b) => b.playedAt - a.playedAt);

        filteredHistory.forEach((item) => {
          const historyElement = document.createElement("div");
          historyElement.className = "history-item";
          historyElement.innerHTML = `
                <div class="history-info">
                    <div class="song-title">${item.title}</div>
                    <div class="play-date">
                        <i class="far fa-clock"></i>
                        ${formatDate(item.playedAt)} · 
                        ${item.plays} ${item.plays === 1 ? "reproducción" : "reproducciones"}
                    </div>
                </div>
                <div class="history-actions">
                    <span>${item.duration}</span>
                    <i class="fas fa-play play-btn" title="Reproducir"></i>
                    <i class="fas fa-plus-circle add-to-playlist" title="Añadir a playlist"></i>
                </div>
            `;

          historyContainer.appendChild(historyElement);
        });
      } catch (error) {
        console.warn(error.message);
      }
    }

    loadHistory();
  },
};
