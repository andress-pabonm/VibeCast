window.views = window.views || {};

window.views.biblioteca = {
  title: `<i class="fas fa-music"></i>Biblioteca`,
  html: `
    <section>
      <h2>Biblioteca</h2>
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
