document.addEventListener("DOMContentLoaded", () => {
  const views = window.views || {};
  const sidebarNav = getElement(document, ".sidebar nav");
  const mainContent = getElement(document, ".main-content");

  let activeView = null;

  Object.entries(views).forEach(([viewName, view]) => {
    const btn = document.createElement("button");

    btn.id = `btn-${viewName}`;
    btn.innerHTML = view.title || viewName;
    btn.classList.add("menu-btn");

    btn.addEventListener("click", () => {
      // Marcar botón activo
      getElements(sidebarNav, "button").forEach((b) =>
        b.classList.remove("active")
      );
      btn.classList.add("active");

      // Cargar contenido HTML
      if (view?.html) {
        mainContent.innerHTML = view.html;

        // Cargar CSS de la vista
        setViewStylesheet(view.style);

        if (typeof view.init === "function") {
          view.init();
        }
        activeView = viewName;
      } else {
        mainContent.innerHTML = `<p>Vista "${viewName}" no encontrada.</p>`;
      }
    });

    sidebarNav.appendChild(btn);
  });

  // Activar la primera vista automáticamente
  const firstView = Object.keys(views)[0];
  if (firstView) {
    const firstBtn = getElement(sidebarNav, `#btn-${firstView}`);
    firstBtn?.click();
  }
});

function setViewStylesheet(href) {
  // Elimina estilos anteriores
  const existing = getElement(document, "#view-style");
  if (existing) existing.remove();

  if (href) {
    const link = document.createElement("link");
    link.id = "view-style";
    link.rel = "stylesheet";
    link.href = href;
    document.head.appendChild(link);
  }
}
