window.views = window.views || {};

window.views.perfil = {
  title: `<i class="fas fa-user"></i>Perfil`,
  html: `
    <header class="profile-header">
        <!-- En la sección de información del perfil -->
        <div class="profile-info">
            <div class="profile-details">
                <h1 id="profile-name"></h1>
                <p id="profile-username"></p>
                <p id="profile-country"><i class="fas fa-globe"></i></p>
                <p id="profile-email"><i class="fas fa-envelope"></i></p>

                <button id="becomeArtistBtn" class="action-btn">
                    <i class="fas fa-microphone"></i> Convertirse en artista
                </button>
            </div>
            <button class="logout-btn" id="logoutBtn">
                <i class="fas fa-sign-out-alt"></i>Cerrar sesión
            </button>
        </div>
    </header>

    <div class="profile-content">
        <!-- Sección de información básica -->
        <section class="profile-section">
            <h2 class="section-title">
                <i class="fas fa-user-edit"></i>Tu información
            </h2>
            <form id="profileForm" class="profile-form">
                <div class="form-group">
                    <label for="edit-name">Nombre</label>
                    <input type="text" id="edit-name" placeholder="Tu nombre" required />
                </div>
                <div class="form-group">
                    <label for="edit-country">País</label>
                    <input type="text" id="edit-country" placeholder="Tu país" required />
                </div>
                <div class="form-group">
                    <label for="edit-username">Nombre de usuario</label>
                    <input type="text" id="edit-username" placeholder="Tu nombre de usuario" required />
                </div>
                <div class="form-group">
                    <label for="edit-country">Correo electrónico</label>
                    <input type="email" id="edit-email" placeholder="Tu correo electrónico" required />
                </div>
                <button type="submit" class="save-btn">Guardar cambios</button>
            </form>
        </section>

        <!-- Sección de cambio de contraseña -->
        <section class="profile-section">
            <h2 class="section-title">
                <i class="fas fa-lock"></i> Cambiar contraseña
            </h2>
            <form id="passwordForm" class="password-form">
                <div class="form-group">
                    <label for="current-password">Contraseña actual</label>
                    <div class="input-with-icon">
                        <input type="password" id="current-password" placeholder="Contraseña actual" required />
                        <i class="fas fa-eye toggle-password"></i>
                    </div>
                </div>
                <div class="form-group">
                    <label for="new-password">Nueva contraseña</label>
                    <div class="input-with-icon">
                        <input type="password" id="new-password" placeholder="Nueva contraseña" required />
                        <i class="fas fa-eye toggle-password"></i>
                    </div>
                </div>
                <div class="form-group">
                    <label for="confirm-new-password">Confirmar nueva contraseña</label>
                    <div class="input-with-icon">
                        <input type="password" id="confirm-new-password" placeholder="Confirmar contraseña" required />
                        <i class="fas fa-eye toggle-password"></i>
                    </div>
                </div>
                <button type="submit" class="save-btn">Cambiar contraseña</button>
            </form>
        </section>

        <!-- Sección de suscripción -->
        <section class="profile-section">
            <h2 class="section-title">
                <i class="fas fa-crown"></i>Tu suscripción
            </h2>
            <div class="subscription-info" id="subscriptionInfo">
                <!-- Información de suscripción se cargará dinámicamente -->
            </div>
            <button class="upgrade-btn" id="upgradeBtn">
                <i class="fas fa-arrow-up"></i>Actualizar plan
            </button>
        </section>

        <!-- Sección de artista (solo visible si es artista) -->
        <section class="artist-section hidden" id="artistSection">
            <h2 class="section-title">
                <i class="fas fa-compact-disc"></i> Tus discos
            </h2>
            <div class="albums-grid" id="albumsContainer">
                <!-- Álbumes se cargarán dinámicamente -->
            </div>
            <button class="new-album-btn" id="newAlbumBtn">
                <i class="fas fa-plus"></i> Nuevo disco
            </button>
        </section>

        <!-- Sección de Canciones del Álbum -->
        <section class="songs-section hidden" id="albumSongsSection">
            <div class="songs-header">
                <button class="back-btn" id="backToAlbums">
                    <i class="fas fa-arrow-left"></i>
                </button>
                <h2 id="currentAlbumTitle"></h2>
                <button class="action-btn" id="addAlbumToQueueBtn">
                    <i class="fas fa-plus-circle"></i> Añadir a cola
                </button>
            </div>
            <div class="song-list" id="albumSongsContainer">
                <!-- Canciones del álbum se cargarán dinámicamente -->
            </div>
        </section>

        <!-- Modal para editar álbum -->
        <div class="modal hidden" id="editAlbumModal">
            <div class="modal-content">
                <span class="close-modal">&times;</span>
                <h3>Editar álbum</h3>
                <div class="form-group">
                    <label for="edit-album-name">Nombre del álbum</label>
                    <input type="text" id="edit-album-name" placeholder="Nombre del álbum" required />
                </div>
                <div class="form-group">
                    <label for="edit-album-year">Año de lanzamiento</label>
                    <input type="text" id="edit-album-year" placeholder="Año" required />
                </div>
                <div class="modal-buttons">
                    <button id="saveAlbumBtn">Guardar cambios</button>
                    <button id="deleteAlbumBtn">Eliminar álbum</button>
                </div>
            </div>
        </div>
    </div>

    <!-- Modal de planes -->
    <div class="modal hidden" id="plansModal">
        <div class="modal-content">
            <span class="close-modal">&times;</span>
            <h3>Elige tu plan</h3>
            <div class="plans-container" id="plansContainer">
                <!-- Planes se cargarán dinámicamente -->
            </div>
        </div>
    </div>

    <!-- Modal para nuevo disco -->
    <div class="modal hidden" id="newAlbumModal">
        <div class="modal-content">
            <span class="close-modal">&times;</span>
            <h3>Crear nuevo disco</h3>
            <div class="form-group">
                <label for="album-name">Nombre del disco</label>
                <input type="text" id="album-name" placeholder="Nombre del disco" required />
            </div>
            <div class="form-group">
                <label for="album-year">Año de lanzamiento</label>
                <input type="text" id="album-year" placeholder="Año" required />
            </div>
            <button id="createAlbumBtn">Crear disco</button>
        </div>
    </div>

  `,
  style: "../styles/perfilStyles.css",
  init: function () {
    document.addEventListener("DOMContentLoaded", async () => {
      try {
        const res = await window.get_user_data();

        if (!res || res.status !== "ok" || res.type !== "json" || !res.data) {
          throw new Error(res?.message || "No se pudo cargar el perfil");
        }

        const userData = res.data;

        // Inicializa la vista base del perfil
        initializeProfile(userData);

        // Si el usuario es artista, cargar datos de álbumes
        if (userData.isArtist) {
          await loadArtistData(userData);
        }
      } catch (err) {
        console.error("Error al cargar perfil:", err);
        alert("No se pudo cargar el perfil, redirigiendo...");
        // window.location.replace("../Login/index.html");
      }
    });

    async function loadArtistData(userData) {
      try {
        const artistRes = await window.get_artist_data(userData.id);
        if (artistRes.status === "ok" && artistRes.type === "json") {
          userData.albums = artistRes.data.albums || [];
          setupArtistView(userData);
        }
      } catch (err) {
        console.error("Error al cargar datos de artista:", err);
      }
    }

    function initializeProfile(userData) {
      setProfileFields(userData);
      setSubscriptionInfo(userData);
      setupProfileForm(userData);
      setupPasswordForm();
      setupLogout();
      setupPasswordToggle();
      setupBecomeArtistButton(userData);

      document.getElementById("upgradeBtn").addEventListener("click", () => {
        loadPlans(userData);
        document.getElementById("plansModal").classList.remove("hidden");
      });

      document.getElementById("newAlbumBtn")?.addEventListener("click", () => {
        document.getElementById("newAlbumModal").classList.remove("hidden");
      });

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

    function setupBecomeArtistButton(userData) {
      const becomeArtistBtn = document.getElementById("becomeArtistBtn");
      if (!becomeArtistBtn) return;

      if (userData.isArtist) {
        becomeArtistBtn.style.display = "none";
      } else {
        becomeArtistBtn.addEventListener("click", async () => {
          try {
            const res = await window.become_artist();
            if (res.status === "ok") {
              alert(
                "¡Ahora eres un artista! Se ha creado tu primer álbum y canción."
              );
              location.reload();
            } else {
              alert(
                "Error al convertirte en artista: " +
                (res.message || "Error desconocido")
              );
            }
          } catch (err) {
            console.error("Error:", err);
            alert("Error al procesar la solicitud");
          }
        });
      }
    }

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
      <button class="select-plan-btn ${disabled}" data-plan="${plan.id
          }">${btnText}</button>
    `;
        container.appendChild(card);
      });

      container.addEventListener("click", (e) => {
        if (!e.target.classList.contains("select-plan-btn")) return;
        if (e.target.classList.contains("current")) return;

        const selected = e.target.getAttribute("data-plan");
        if (confirm(`¿Cambiar al plan ${selected}?`)) {
          userData.subscription.type = selected;
          userData.subscription.autoRenewal = selected !== "free";
          const expiration = new Date();
          selected === "annual"
            ? expiration.setFullYear(expiration.getFullYear() + 1)
            : expiration.setMonth(expiration.getMonth() + 1);
          userData.subscription.expiration = expiration.toISOString().split("T")[0];

          setSubscriptionInfo(userData);
          document.getElementById("plansModal").classList.add("hidden");
          alert(`Ahora tienes el plan ${selected}`);
        }
      });
    }

    function setupArtistView(user) {
      const section = document.getElementById("artistSection");
      section.classList.remove("hidden");

      let artistAlbums = user.albums || [];

      // Si no tiene álbumes, crear uno por defecto
      if (artistAlbums.length === 0) {
        artistAlbums.push({
          id: Date.now(),
          name: "Mi primer álbum",
          year: new Date().getFullYear().toString(),
          genre: "General",
          songs: [
            {
              id: Date.now() + 1,
              title: "Mi primera canción",
              artist: user.name,
              duration: "3:45",
            },
          ],
        });
        user.albums = artistAlbums;
      }

      renderAlbums(artistAlbums);
      setupAlbumEvents();
    }

    function renderAlbums(albums) {
      const albumsContainer = document.getElementById("albumsContainer");
      albumsContainer.innerHTML = "";

      albums.forEach((album) => {
        const el = document.createElement("div");
        el.className = "album-card";
        el.innerHTML = `
      <div class="album-options" data-id="${album.id
          }"><i class="fas fa-ellipsis-h"></i></div>
      <div class="album-image"><i class="fas fa-compact-disc"></i></div>
      <div class="album-info">
        <h3 class="album-name">${album.name}</h3>
        <div class="album-details"><span>${album.year}</span><span>${album.genre
          }</span></div>
        <div class="album-details"><span>${album.songs.length} canción${album.songs.length !== 1 ? "es" : ""
          }</span></div>
      </div>
    `;

        el.querySelector(".album-options").addEventListener("click", (e) => {
          e.stopPropagation();
          currentAlbumId = album.id;
          document.getElementById("edit-album-name").value = album.name;
          document.getElementById("edit-album-year").value = album.year;
          document.getElementById("editAlbumModal").classList.remove("hidden");
        });

        el.addEventListener("click", () => {
          showAlbumSongs(album.id, album.name, album.songs);
        });

        albumsContainer.appendChild(el);
      });
    }

    function showAlbumSongs(id, name, songs) {
      const albumSongsSection = document.getElementById("albumSongsSection");
      const albumSongsContainer = document.getElementById("albumSongsContainer");
      const currentAlbumTitle = document.getElementById("currentAlbumTitle");

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

      document.getElementById("artistSection").classList.add("hidden");
      albumSongsSection.classList.remove("hidden");
    }

    function setupAlbumEvents() {
      const backToAlbums = document.getElementById("backToAlbums");
      const createAlbumBtn = document.getElementById("createAlbumBtn");
      const saveAlbumBtn = document.getElementById("saveAlbumBtn");
      const deleteAlbumBtn = document.getElementById("deleteAlbumBtn");
      const addAlbumToQueueBtn = document.getElementById("addAlbumToQueueBtn");

      backToAlbums?.addEventListener("click", () => {
        document.getElementById("albumSongsSection").classList.add("hidden");
        document.getElementById("artistSection").classList.remove("hidden");
      });

      createAlbumBtn?.addEventListener("click", () => {
        const name = document.getElementById("album-name").value.trim();
        const year = document.getElementById("album-year").value.trim();

        if (!name || !year) {
          alert("Completa todos los campos");
          return;
        }

        const newAlbum = {
          id: Date.now(),
          name,
          year,
          genre: "General",
          songs: [],
        };

        // Aquí deberías llamar a una función del backend para crear el álbum
        alert("Funcionalidad de creación de álbum no implementada aún");
        document.getElementById("newAlbumModal").classList.add("hidden");
      });

      saveAlbumBtn?.addEventListener("click", () => {
        const name = document.getElementById("edit-album-name").value.trim();
        const year = document.getElementById("edit-album-year").value.trim();

        if (!name || !year) {
          alert("Completa todos los campos");
          return;
        }

        // Aquí deberías llamar a una función del backend para actualizar el álbum
        alert("Funcionalidad de edición de álbum no implementada aún");
        document.getElementById("editAlbumModal").classList.add("hidden");
      });

      deleteAlbumBtn?.addEventListener("click", () => {
        if (confirm("¿Eliminar este álbum?")) {
          // Aquí deberías llamar a una función del backend para eliminar el álbum
          alert("Funcionalidad de eliminación de álbum no implementada aún");
          document.getElementById("editAlbumModal").classList.add("hidden");
        }
      });

      addAlbumToQueueBtn?.addEventListener("click", () => {
        // Aquí deberías implementar la lógica para añadir el álbum a la cola de reproducción
        alert("Funcionalidad de añadir álbum a cola no implementada aún");
      });
    }

  },
};
