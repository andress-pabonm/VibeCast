document.addEventListener("DOMContentLoaded", async () => {
  try {
    const session = await window.is_logged_in();
    if (session.status !== "ok" || session.data !== true) {
      window.location.replace("../Login/index.html");
      return;
    }

    const res = await window.get_user_data();
    if (res.status !== "ok" || res.type !== "json") {
      throw new Error(res.message || "No se pudo cargar el perfil");
    }

    const userData = res.data;
    initializeProfile(userData);
  } catch (err) {
    console.error("Error al cargar perfil:", err);
    window.location.replace("../Login/index.html");
  }
});

// ========= Función Principal =========
function initializeProfile(userData) {
  // Mostrar info en el DOM
  setProfileFields(userData);
  setSubscriptionInfo(userData);
  setupProfileForm(userData);
  setupPasswordForm();
  setupLogout();
  setupPasswordToggle();

  // Cargar vista artista
  if (userData.isArtist) {
    setupArtistView(userData);
  }

  // Abrir modales
  document.getElementById("upgradeBtn").addEventListener("click", () => {
    loadPlans(userData);
    document.getElementById("plansModal").classList.remove("hidden");
  });

  document.getElementById("newAlbumBtn").addEventListener("click", () => {
    document.getElementById("newAlbumModal").classList.remove("hidden");
  });

  // Cerrar modales
  document.querySelectorAll(".close-modal").forEach((btn) => {
    btn.addEventListener("click", () => {
      ["plansModal", "newAlbumModal", "editAlbumModal"].forEach((id) =>
        document.getElementById(id).classList.add("hidden")
      );
    });
  });

  ["plansModal", "newAlbumModal", "editAlbumModal"].forEach((id) => {
    document.getElementById(id).addEventListener("click", (e) => {
      if (e.target.id === id) {
        e.target.classList.add("hidden");
      }
    });
  });
}

// ========= Perfil =========
function setProfileFields(user) {
  document.getElementById("profile-name").textContent = user.name;
  document.getElementById("profile-username").textContent = user.username;
  document.getElementById(
    "profile-country"
  ).innerHTML = `<i class="fas fa-globe"></i> ${user.country}`;
  document.getElementById(
    "profile-email"
  ).innerHTML = `<i class="fas fa-envelope"></i> ${user.email}`;

  document.getElementById("edit-name").value = user.name;
  document.getElementById("edit-username").value = user.username;
  document.getElementById("edit-country").value = user.country;
  document.getElementById("edit-email").value = user.email;
}

function setupProfileForm(userData) {
  const form = document.getElementById("profileForm");
  form.addEventListener("submit", (e) => {
    e.preventDefault();

    const newName = document.getElementById("edit-name").value.trim();
    const newUsername = document.getElementById("edit-username").value.trim();
    const newCountry = document.getElementById("edit-country").value.trim();
    const newEmail = document.getElementById("edit-email").value.trim();

    if (!newUsername.startsWith("@")) {
      alert("El nombre de usuario debe comenzar con @");
      return;
    }

    Object.assign(userData, {
      name: newName,
      username: newUsername,
      country: newCountry,
      email: newEmail,
    });

    setProfileFields(userData);
    alert("Perfil actualizado correctamente");
  });
}

// ========= Contraseña =========
function setupPasswordForm() {
  const form = document.getElementById("passwordForm");
  form.addEventListener("submit", (e) => {
    e.preventDefault();

    const pass1 = document.getElementById("new-password").value;
    const pass2 = document.getElementById("confirm-new-password").value;

    const valid =
      pass1 === pass2 &&
      pass1.length >= 8 &&
      /[A-Z]/.test(pass1) &&
      /[a-z]/.test(pass1) &&
      /\d/.test(pass1) &&
      /[^A-Za-z0-9]/.test(pass1);

    if (!valid) {
      alert(
        "La contraseña debe tener al menos 8 caracteres, incluyendo mayúscula, minúscula, número y símbolo."
      );
      return;
    }

    alert("Contraseña cambiada correctamente");
    form.reset();
  });
}

function setupPasswordToggle() {
  document.querySelectorAll(".toggle-password").forEach((btn) => {
    btn.addEventListener("click", function () {
      const input = this.closest(".input-with-icon").querySelector("input");
      input.type = input.type === "password" ? "text" : "password";
      this.classList.toggle("fa-eye");
      this.classList.toggle("fa-eye-slash");
    });
  });
}

// ========= Sesión =========
function setupLogout() {
  document.getElementById("logoutBtn").addEventListener("click", async () => {
    try {
      const res = await window.cerrar_sesion();
      if (res.status === "ok") {
        window.location.replace("../Login/index.html");
      }
    } catch (err) {
      console.error("Error al cerrar sesión:", err);
    }
  });
}

// ========= Suscripción =========
function setSubscriptionInfo(user) {
  const info = document.getElementById("subscriptionInfo");
  const isPremium = ["premium", "annual"].includes(user.subscription.type);
  const plan = user.subscription.type;
  const planInfo =
    subscriptionPlans.find((p) => p.id === plan) || subscriptionPlans[0];

  let renewal = "";
  if (isPremium) {
    renewal = user.subscription.autoRenewal
      ? '<i class="fas fa-sync-alt"></i> Renovación automática'
      : "";
  }

  info.innerHTML = `
    <div class="subscription-status ${plan}">
      <i class="fas ${isPremium ? "fa-crown" : "fa-music"}"></i>
      <span>Plan ${planInfo.name}</span>
    </div>
    <div class="expiration-date">${renewal}</div>
    <div class="subscription-features">
      <h4>${isPremium ? "Beneficios:" : "Actualiza para obtener:"}</h4>
      <ul>
        ${planInfo.features
          .map((f) => `<li><i class="fas fa-check"></i> ${f}</li>`)
          .join("")}
      </ul>
    </div>
  `;

  const upgradeBtn = document.getElementById("upgradeBtn");
  upgradeBtn.innerHTML = isPremium
    ? `<i class="fas fa-sync-alt"></i> Renovar plan`
    : `<i class="fas fa-arrow-up"></i> Actualizar a Premium`;
}

// ========= Planes =========
const subscriptionPlans = [
  {
    id: "free",
    name: "Free",
    price: 0,
    period: "Gratis",
    features: ["Con anuncios", "Límite de saltos"],
  },
  {
    id: "premium",
    name: "Premium",
    price: 4.99,
    period: "por mes",
    features: ["Sin anuncios", "Saltos ilimitados"],
  },
  {
    id: "annual",
    name: "Premium Anual",
    price: 49.99,
    period: "por año",
    features: ["Sin anuncios", "Saltos ilimitados", "12 meses de música"],
  },
];

function loadPlans(userData) {
  const container = document.getElementById("plansContainer");
  container.innerHTML = "";

  subscriptionPlans.forEach((plan) => {
    const isCurrent = plan.id === userData.subscription.type;
    const btnText = isCurrent ? "Plan actual" : `Elegir ${plan.name}`;
    const disabled = isCurrent ? "current" : "";

    const card = document.createElement("div");
    card.className = `plan-card ${plan.id}`;
    card.innerHTML = `
      <h3 class="plan-name">${plan.name}</h3>
      <div class="plan-price">$${plan.price.toFixed(2)}</div>
      <div class="plan-period">${plan.period}</div>
      <ul class="plan-features">
        ${plan.features
          .map((f) => `<li><i class="fas fa-check"></i> ${f}</li>`)
          .join("")}
      </ul>
      <button class="select-plan-btn ${disabled}" data-plan="${
      plan.id
    }">${btnText}</button>
    `;
    container.appendChild(card);
  });

  container.addEventListener(
    "click",
    (e) => {
      if (!e.target.classList.contains("select-plan-btn")) return;

      const selected = e.target.getAttribute("data-plan");
      if (e.target.classList.contains("current")) return;

      if (confirm(`¿Cambiar al plan ${selected}?`)) {
        userData.subscription.type = selected;
        userData.subscription.autoRenewal = selected !== "free";
        const expiration = new Date();
        selected === "annual"
          ? expiration.setFullYear(expiration.getFullYear() + 1)
          : expiration.setMonth(expiration.getMonth() + 1);
        userData.subscription.expiration = expiration
          .toISOString()
          .split("T")[0];

        setSubscriptionInfo(userData);
        document.getElementById("plansModal").classList.add("hidden");
        alert(`Ahora tienes el plan ${selected}`);
      }
    },
    { once: true }
  );
}

// ========= Vista de Artista =========
function setupArtistView(user) {
  const section = document.getElementById("artistSection");
  section.classList.remove("hidden");

  let artistAlbums = user.albums || [];

  const albumsContainer = document.getElementById("albumsContainer");
  const newAlbumModal = document.getElementById("newAlbumModal");
  const editAlbumModal = document.getElementById("editAlbumModal");

  const albumSongsSection = document.getElementById("albumSongsSection");
  const albumSongsContainer = document.getElementById("albumSongsContainer");
  const currentAlbumTitle = document.getElementById("currentAlbumTitle");
  const backToAlbums = document.getElementById("backToAlbums");

  let currentAlbumId = null;

  function renderAlbums() {
    albumsContainer.innerHTML = "";
    artistAlbums.forEach((album) => {
      const el = document.createElement("div");
      el.className = "album-card";
      el.innerHTML = `
        <div class="album-options" data-id="${
          album.id
        }"><i class="fas fa-ellipsis-h"></i></div>
        <div class="album-image"><i class="fas fa-compact-disc"></i></div>
        <div class="album-info">
          <h3 class="album-name">${album.name}</h3>
          <div class="album-details"><span>${album.year}</span><span>${
        album.genre
      }</span></div>
          <div class="album-details"><span>${album.songs.length} canción${
        album.songs.length !== 1 ? "es" : ""
      }</span></div>
        </div>
      `;

      el.querySelector(".album-options").addEventListener("click", (e) => {
        e.stopPropagation();
        currentAlbumId = album.id;
        document.getElementById("edit-album-name").value = album.name;
        document.getElementById("edit-album-year").value = album.year;
        editAlbumModal.classList.remove("hidden");
      });

      el.addEventListener("click", () => {
        showAlbumSongs(album.id, album.name, album.songs);
      });

      albumsContainer.appendChild(el);
    });
  }

  function showAlbumSongs(id, name, songs) {
    currentAlbumId = id;
    currentAlbumTitle.textContent = name;
    albumSongsContainer.innerHTML = "";

    songs.forEach((song) => {
      const el = document.createElement("div");
      el.className = "song-item";
      el.innerHTML = `
        <div class="song-info"><strong>${song.title}</strong><p>${song.artist}</p></div>
        <div class="song-actions"><span>${song.duration}</span><i class="fas fa-play play-btn" title="Reproducir"></i></div>
      `;
      albumSongsContainer.appendChild(el);
    });

    section.classList.add("hidden");
    albumSongsSection.classList.remove("hidden");
  }

  backToAlbums.addEventListener("click", () => {
    albumSongsSection.classList.add("hidden");
    section.classList.remove("hidden");
  });

  document.getElementById("createAlbumBtn").addEventListener("click", () => {
    const name = document.getElementById("album-name").value.trim();
    const year = document.getElementById("album-year").value.trim();
    const genre = document.getElementById("album-genre").value.trim();

    if (!name || !year || !genre) {
      alert("Completa todos los campos");
      return;
    }

    const newAlbum = {
      id: Date.now(),
      name,
      year,
      genre,
      songs: [],
    };

    artistAlbums.push(newAlbum);
    renderAlbums();
    newAlbumModal.classList.add("hidden");
  });

  document.getElementById("saveAlbumBtn").addEventListener("click", () => {
    const name = document.getElementById("edit-album-name").value.trim();
    const year = document.getElementById("edit-album-year").value.trim();

    const album = artistAlbums.find((a) => a.id === currentAlbumId);
    if (album) {
      album.name = name;
      album.year = year;
      renderAlbums();
      editAlbumModal.classList.add("hidden");
    }
  });

  document.getElementById("deleteAlbumBtn").addEventListener("click", () => {
    if (confirm("¿Eliminar este álbum?")) {
      artistAlbums = artistAlbums.filter((a) => a.id !== currentAlbumId);
      renderAlbums();
      editAlbumModal.classList.add("hidden");
    }
  });

  renderAlbums();
}
