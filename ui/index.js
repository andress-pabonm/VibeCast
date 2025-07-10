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

  // Botones
  const buttons = Object.fromEntries(
    sections.map((name) => [name, getElement(`#btn-${name}`)])
  );

  function showView(viewName) {
    sections.forEach((section) => {
      buttons[section].classList.remove("active");
    });
    // getElements(".sidebar nav button").forEach((btn) => {
    //   btn.classList.remove("active");
    // });
    buttons[viewName].classList.add("active");

    for (const key in views) {
      views[key].classList.toggle("hidden", key !== viewName);
    }
  }

  sections.forEach((section) => {
    buttons[section].addEventListener("click", (e) => showView(section));
  });

  showView(sections[0]);
});
