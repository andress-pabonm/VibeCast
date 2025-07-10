window.views = window.views || {};
window.views.inicio = {
  title: `<i class="fas fa-home"></i>Inicio`,
  html: `
    <header class="main-header">
      <div class="search-bar">
        <i class="fas fa-search search-icon"></i>
        <input
          type="text"
          placeholder="Buscar canciones, artistas..."
          class="search-input"
        />
      </div>
    </header>

    <div class="content-grid">
      <!-- Canciones de la base de datos -->
      <section class="card-section database-songs">
        <h2 class="section-title">
          <i class="fas fa-database"></i>Explorar Canciones
        </h2>
        <div class="song-list" id="songs-container">
          <!-- Canciones se cargarán dinámicamente -->
        </div>
      </section>

      <!-- Recomendaciones de amigos -->
      <section class="card-section friend-recommendations">
        <h2 class="section-title">
          <i class="fas fa-heart"></i>Recomendaciones
        </h2>
        <div class="song-list" id="recommendations-container">
          <!-- Canciones recomendadas se cargarán dinámicamente -->
        </div>
      </section>

      <!-- Estadisticas del usuario -->
      <section class="card-section friends">
        <h2 class="section-title">
          <i class="fas fa-chart-line"></i>Estadisticas
        </h2>
        <div class="friends-list" id="friends-container">
          <!-- Estadisticas se cargarán dinámicamente -->
        </div>
      </section>
    </div>
  `,
  style: "styles/inicio.css",
  init: function () {
    const btn = document.getElementById("btn-saludo");
    if (btn) {
      btn.addEventListener("click", () => {
        alert("¡Hola desde la vista de inicio!");
      });
    }
  },
};
