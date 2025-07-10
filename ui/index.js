// Para obtener el primer elemento que coincide con un selector
const getElement = (selector) => document.querySelector(selector);

// Para obtener todos los elementos que coinciden con un selector
const getElements = (selector) => document.querySelectorAll(selector);

document.addEventListener("DOMContentLoaded", () => {
  // Secciones
  const sections = ["inicio", "biblioteca", "historial", "amigos", "perfil"];

  // Vistas
  const views = Object.fromEntries(
    sections.map((name) => [name, getElement(`#view-${name}`)])
  );

  function showView(viewName) {
    getElements(".sidebar nav button").forEach((btn) => {
      btn.classList.remove("active");
    });
    buttons[viewName].classList.add("active");

    for (const key in views) {
      views[key].classList.toggle("hidden", key !== viewName);
    }
  }
});

document
  .getElementById("btn-inicio")
  .addEventListener("click", () => showView("inicio"));
document
  .getElementById("btn-biblioteca")
  .addEventListener("click", () => showView("biblioteca"));
document
  .getElementById("btn-historial")
  .addEventListener("click", () => showView("historial"));
document
  .getElementById("btn-amigos")
  .addEventListener("click", () => showView("amigos"));
document
  .getElementById("btn-perfil")
  .addEventListener("click", () => showView("perfil"));

// Cargar vista por defecto
showView("inicio");
