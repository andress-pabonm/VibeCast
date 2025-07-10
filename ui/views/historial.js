window.views = window.views || {};

window.views.historial = {
  title: `<i class="fas fa-history"></i>Historial`,
  html: `
    <section>
      <h2>Inicio</h2>
      <p>Bienvenido a VibeCast 🎧</p>
      <button id="btn-saludo">¡Haz clic!</button>
    </section>
  `,
  init: function () {
    const btn = document.getElementById("btn-saludo");
    if (btn) {
      btn.addEventListener("click", () => {
        alert("¡Hola desde la vista de inicio!");
      });
    }
  },
};
