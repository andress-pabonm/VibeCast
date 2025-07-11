document.addEventListener("DOMContentLoaded", () => {
  const views = window.views || {};
  const sidebarNav = getElement(document, ".sidebar nav");
  const mainContent = getElement(document, ".main-content");

  // Crear los botones del menú
  Object.entries(views).forEach(([viewName, view]) => {
    const btn = document.createElement("button");

    btn.id = `btn-${viewName}`;
    btn.innerHTML = view.title || viewName;
    btn.classList.add("menu-btn");

    btn.addEventListener("click", () => {
      // Cambiar el hash en la URL
      location.hash = `#${viewName}`;
    });

    sidebarNav.appendChild(btn);
  });

  // Función para cargar una vista
  function showView(viewName) {
    const view = views[viewName];
    if (!view) {
      mainContent.innerHTML = `<p>Vista "${viewName}" no encontrada.</p>`;
      return;
    }

    // Marcar botón activo
    getElements(sidebarNav, "button").forEach((b) =>
      b.classList.remove("active")
    );
    const btn = getElement(sidebarNav, `#btn-${viewName}`);
    if (btn) btn.classList.add("active");

    // Cargar contenido HTML
    mainContent.innerHTML = view.html;

    // Inicializar lógica de la vista
    if (typeof view.init === "function") {
      view.init();
    }
  }

  // Escuchar cambios en el hash de la URL
  window.addEventListener("hashchange", () => {
    const viewName = location.hash.replace("#", "");
    if (viewName in views) {
      showView(viewName);
    }
  });

  // Cargar la vista inicial según el hash o la primera disponible
  const initialView = location.hash.replace("#", "") || Object.keys(views)[0];
  if (initialView in views) {
    showView(initialView);
    location.hash = `#${initialView}`; // asegura consistencia en la URL
  }
});
