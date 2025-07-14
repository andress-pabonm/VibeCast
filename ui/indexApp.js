document.addEventListener("DOMContentLoaded", async () => {
  const res = await window.sesion_activa();
  if (res.data === false) {
    window.location.replace("pages/login.html");
  }
  console.log(res.message);

  const views = window.views || {};
  const sidebarNav = getElement(".sidebar nav");
  const mainContent = getElement(".main-content");

  // console.log("views: ", views);

  // Crear los botones del menú
  const btns = Object.fromEntries(
    Object.entries(views).map(([viewName, view]) => {
      const btn = document.createElement("button");

      btn.id = `btn-${viewName}`;
      btn.innerHTML = view.title || viewName;
      btn.classList.add("menu-btn");

      btn.addEventListener("click", () => {
        // Cambiar el hash en la URL
        window.location.hash = `#${viewName}`;
      });

      sidebarNav.appendChild(btn);

      return [viewName, btn];
    })
  );

  // Función para cargar una vista
  function showView(viewName) {
    const view = views[viewName];
    if (!view) {
      mainContent.innerHTML = `<p>Vista "${viewName}" no encontrada.</p>`;
      return;
    }

    // Marcar botón activo
    Object.values(btns).forEach((b) => b.classList.remove("active"));
    btns[viewName].classList.add("active");

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
