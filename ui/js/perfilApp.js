window.views = window.views || {};

window.views.perfil = {
  title: `<i class="fas fa-user"></i>Perfil`,
  html: `
    <header class="profile-header">
        <!-- Sección de información del perfil -->
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
            <h2 class="section-title"><i class="fas fa-user-edit"></i>Tu información</h2>
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
            <h2 class="section-title"><i class="fas fa-lock"></i> Cambiar contraseña</h2>
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
            <h2 class="section-title"><i class="fas fa-crown"></i>Tu suscripción</h2>
            <div class="subscription-info" id="subscriptionInfo"></div>
            <button class="upgrade-btn" id="upgradeBtn">
                <i class="fas fa-arrow-up"></i>Actualizar plan
            </button>
        </section>

        <!-- Sección de eliminación -->
        <section class="profile-section delete-section">
          <h2 class="section-title"><i class="fas fa-exclamation-triangle"></i>Zona peligrosa</h2>
          <div class="delete-options">
            <button class="delete-option-btn delete-account-btn" id="deleteUserBtn">
            <i class="fas fa-user-slash"></i> Eliminar cuenta
            </button>
            <button class="delete-option-btn delete-artist-btn" id="deleteArtistBtn">
              <i class="fas fa-microphone-slash"></i> Dejar de ser artista
            </button>
          </div>
        </section>

        <!-- Sección de artista -->
        <section class="artist-section hidden" id="artistSection">
            <h2 class="section-title"><i class="fas fa-compact-disc"></i> Tus discos</h2>
            <div class="albums-grid" id="albumsContainer"></div>
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
                <button class="action-btn" id="addCancion">
                    <i class="fas fa-music"></i> Nueva canción
                </button>
            </div>
            <div class="song-list" id="albumSongsContainer"></div>
        </section>

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
            <button id="createAlbumBtn">Crear disco</button>
        </div>
    </div>

    <!-- Modal para editar álbum -->
    <div class="modal hidden" id="editAlbumModal">
        <div class="modal-content">
            <span class="close-modal">&times;</span>
            <h3>Editar álbum</h3>
            <div class="form-group">
                <label for="edit-album-name">Nombre del álbum</label>
                <input type="text" id="edit-album-name" placeholder="Nombre del álbum" required />
            </div>
            <div class="modal-buttons">
                <button id="saveAlbumBtn">Guardar cambios</button>
                <button id="deleteAlbumBtn">Eliminar álbum</button>
            </div>
        </div>
    </div>

    <!-- Modal de planes -->
    <div class="modal hidden" id="plansModal">
        <div class="modal-content">
            <span class="close-modal">&times;</span>
            <h3>Elige tu plan</h3>
            <div class="plans-container" id="plansContainer"></div>
        </div>
    </div>

    <!-- Modal para nueva canción -->
    <div class="modal hidden" id="newSongModal">
      <div class="modal-content">
        <span class="close-modal">&times;</span>
        <h3>Agregar nueva canción</h3>
        <form id="newSongForm">
          <div class="form-group">
            <label for="song-name">Nombre de la canción</label>
            <input type="text" id="song-name" placeholder="Nombre de la canción" required />
          </div>
          <div class="form-group">
            <label for="song-genre">Género</label>
            <input type="text" id="song-genre" placeholder="Género musical" required />
          </div>
          <div class="form-group">
            <label for="song-duration">Duración (segundos)</label>
            <input type="text" id="song-duration" placeholder="Duración en segundos" required min="1" />
          </div>
          <div class="form-group">
            <label for="song-url">URL de la canción</label>
            <input type="text" id="song-url" placeholder="url de youtube" required />
          </div>
          <button type="submit" class="save-btn">
            <i class="fas fa-save"></i> Guardar canción
          </button>
        </form>
      </div>
    </div>

    <!-- Modal para editar cancion -->
    <div class="modal hidden" id="editCancionModal">
      <div class="modal-content">
        <span class="close-modal">&times;</span>
        <h3>Editar cancion actual</h3>
        <form id="editSongForm">
          <div class="form-group">
            <label for="song-name">Nombre de la canción</label>
            <input type="text" id="song-name" placeholder="Nombre de la canción" required />
          </div>
          <div class="form-group">
            <label for="song-genre">Género</label>
            <input type="text" id="song-genre" placeholder="Género musical" required />
          </div>
          <div class="form-group">
            <label for="song-duration">Duración (segundos)</label>
            <input type="text" id="song-duration" placeholder="Duración en segundos" required min="1" />
          </div>
          <div class="form-group">
            <label for="song-url">URL de la canción</label>
            <input type="text" id="song-url" placeholder="url de youtube" required />
          </div>
          <button type="submit" class="save-btn">
            <i class="fas fa-save"></i> Guardar canción
          </button>
        </form>
      </div>
    </div>

    <!-- Modal para eliminar usuario -->
    <div class="modal hidden" id="deleteUserModal">
      <div class="modal-content">
        <h3>¿Eliminar cuenta permanentemente?</h3>
        <p>Esta acción eliminará todos tus datos y no se podrán recuperar.</p>
        <div class="modal-buttons">
          <button id="confirmDeleteUserBtn" class="danger-btn">Eliminar cuenta</button>
          <button id="cancelDeleteUserBtn" class="cancel-btn">Cancelar</button>
        </div>
      </div>
    </div>

    <!-- Modal para eliminar artista -->
    <div class="modal hidden" id="deleteArtistModal">
      <div class="modal-content">
        <h3>¿Dejar de ser artista?</h3>
        <p>Esta acción eliminará todos tus álbumes y canciones.</p>
        <div class="modal-buttons">
          <button id="confirmDeleteArtistBtn" class="danger-btn">Confirmar</button>
          <button id="cancelDeleteArtistBtn" class="cancel-btn">Cancelar</button>
        </div>
      </div>
    </div>
  `,
  init: (function () {
    // 1. HEADER SECTION FUNCTIONS
    function setProfileFields(user) {
      document.getElementById("profile-name").textContent = user.name;
      document.getElementById("profile-username").textContent = user.username;
      document.getElementById("profile-country").innerHTML = `<i class="fas fa-globe"></i> ${user.country}`;
      document.getElementById("profile-email").innerHTML = `<i class="fas fa-envelope"></i> ${user.email}`;
    }

    function setupBecomeArtistButton(userData) {
      const becomeArtistBtn = document.getElementById("becomeArtistBtn");
      if (!becomeArtistBtn) return;

      if (userData.isArtist) {
        becomeArtistBtn.style.display = "none";
        // Mostrar el botón de dejar de ser artista
        const deleteArtistBtn = document.getElementById("deleteArtistBtn");
        if (deleteArtistBtn) {
          deleteArtistBtn.style.display = "flex";
        }
      } else {
        becomeArtistBtn.addEventListener("click", async () => {
          try {
            const name = prompt("Pon tu nombre de artista: ").trim();
            if (!name) return;

            const res = await window.crear_artista(name);

            if (res.status !== "ok") {
              throw new Error(res.message);
            }

            alert(res.message);

            // Actualizar el estado y mostrar el botón de dejar de ser artista
            userData.isArtist = true;
            becomeArtistBtn.style.display = "none";
            const deleteArtistBtn = document.getElementById("deleteArtistBtn");
            if (deleteArtistBtn) {
              deleteArtistBtn.style.display = "flex";
            }

            // Recargar para mostrar la sección de artista
            window.location.reload();
          } catch (error) {
            alert(error.message);
          }
        });
      }
    }

    async function setupLogout() {
      document.getElementById("logoutBtn").addEventListener("click", async () => {
        try {
          const res = await window.cerrar_sesion();
          if (res.status === "ok") {
            location.replace("pages/login.html");
          }
        } catch (err) {
          console.error("Error al cerrar sesión:", err);
        }
      });
    }

    // 2. PROFILE INFO SECTION FUNCTIONS
    function setupProfileForm(userData) {
      document.getElementById("edit-name").value = userData.name;
      document.getElementById("edit-username").value = userData.username;
      document.getElementById("edit-country").value = userData.country;
      document.getElementById("edit-email").value = userData.email;

      const form = document.getElementById("profileForm");
      form.addEventListener("submit", (e) => {
        e.preventDefault();

        const newName = document.getElementById("edit-name").value.trim();
        const newUsername = document.getElementById("edit-username").value.trim();
        const newCountry = document.getElementById("edit-country").value.trim();
        const newEmail = document.getElementById("edit-email").value.trim();

        const res = safeCall(() => window.actualizar_info_usuario(newName, newCountry, newUsername, newEmail));
        window.location.reload();

        if (res.status !== "ok") return;
        window.location.reload();
      });
    }

    // 3. PASSWORD SECTION FUNCTIONS
    function setupPasswordForm() {
      const form = document.getElementById("passwordForm");
      form.addEventListener("submit", (e) => {
        e.preventDefault();

        const pass1 = document.getElementById("current-password").value;
        const pass2 = document.getElementById("new-password").value;
        const pass3 = document.getElementById("confirm-new-password").value;

        const res = safeCall(() => window.actualizar_password(pass1, pass2, pass3));
        window.location.reload();
        console.log(res.message);
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

    // 4. SUBSCRIPTION SECTION FUNCTIONS
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
    ];

    function setSubscriptionInfo(user) {
      const info = document.getElementById("subscriptionInfo");
      const isPremium = ["premium", "annual"].includes(user.subscription.type);
      const plan = user.subscription.type;
      const planInfo = subscriptionPlans.find((p) => p.id === plan) || subscriptionPlans[0];

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
            ${planInfo.features.map((f) => `<li><i class="fas fa-check"></i> ${f}</li>`).join("")}
          </ul>
        </div>
      `;

      const upgradeBtn = document.getElementById("upgradeBtn");
      upgradeBtn.innerHTML = isPremium
        ? `<i class="fas fa-sync-alt"></i> Renovar plan`
        : `<i class="fas fa-arrow-up"></i> Actualizar a Premium`;
    }

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
            ${plan.features.map((f) => `<li><i class="fas fa-check"></i> ${f}</li>`).join("")}
          </ul>
          <button class="select-plan-btn ${disabled}" data-plan="${plan.id}">${btnText}</button>
        `;
        container.appendChild(card);
      });

      container.addEventListener("click", async (e) => {
        if (!e.target.classList.contains("select-plan-btn") || e.target.classList.contains("current")) return;

        const selected = e.target.getAttribute("data-plan");
        if (confirm(`¿Cambiar al plan ${selected}?`)) {
          try {
            const res = await window.activar_premium(selected === "annual" ? 365 : selected === "premium" ? 30 : 0);

            if (res.status !== "ok")
              throw new Error(res.message);

            alert(res.message);
            document.getElementById("plansModal").classList.add("hidden");

            window.location.reload();
          } catch (error) {
            alert(error.message);
          }
        }
      });
    }

    // 5. ARTIST SECTION FUNCTIONS
    let currentAlbumId = null;
    let currentAlbumName = null;

    async function loadArtistData(userData) {
      try {
        const res = await window.obtener_info_artista();
        if (res.status === "ok" && res.type === "json") {
          userData.albums = res.data.albums || [];
          setupArtistView(userData);
        }
      } catch (error) {
        alert(error.message);
      }
    }

    function setupArtistView(user) {
      const section = document.getElementById("artistSection");
      section.classList.remove("hidden");
      renderAlbums(user.albums || []);
      setupAlbumEvents();
    }

    function setupDeleteButtons(userData) {
      // Eliminar usuario
      document.getElementById("deleteUserBtn")?.addEventListener("click", () => {
        document.getElementById("deleteUserModal").classList.remove("hidden");
      });

      document.getElementById("confirmDeleteUserBtn")?.addEventListener("click", async () => {
        if (confirm("Seguro desea eliminar su cuenta")) {
          try {
            const res = await window.eliminar_cuenta();
            if (res.status !== "ok") throw new Error(res.message);

            alert("Tu cuenta ha sido eliminada exitosamente");
            location.replace("pages/login.html");
          } catch (error) {
            alert(error.message);
          }
        }
      });

      document.getElementById("cancelDeleteUserBtn")?.addEventListener("click", () => {
        document.getElementById("deleteUserModal").classList.add("hidden");
      });

      // Controlar visibilidad del botón "Dejar de ser artista"
      const deleteArtistBtn = document.getElementById("deleteArtistBtn");
      if (deleteArtistBtn) {
        // Mostrar u ocultar basado en isArtist
        deleteArtistBtn.style.display = userData.isArtist ? "flex" : "none";

        // Configurar eventos solo si es artista
        if (userData.isArtist) {
          deleteArtistBtn.addEventListener("click", () => {
            document.getElementById("deleteArtistModal").classList.remove("hidden");
          });

          document.getElementById("confirmDeleteArtistBtn")?.addEventListener("click", async () => {
            if (confirm("Seguro desea dejar de ser artista?")) {
              try {
                const res = await window.eliminar_artista();
                if (res.status !== "ok") throw new Error(res.message);

                alert("Ya no eres artista. Todos tus álbumes y canciones han sido eliminados");
                window.location.reload();
              } catch (error) {
                alert(error.message);
              }
            }
          });

          document.getElementById("cancelDeleteArtistBtn")?.addEventListener("click", () => {
            document.getElementById("deleteArtistModal").classList.add("hidden");
          });
        }
      }
    }

    function renderAlbums(albums) {
      const albumsContainer = document.getElementById("albumsContainer");
      albumsContainer.innerHTML = "";

      albums.forEach((album) => {
        const el = document.createElement("div");
        el.className = "album-card";
        el.innerHTML = `
          <div class="album-options" data-id="${album.id}"><i class="fas fa-ellipsis-h"></i></div>
          <div class="album-image"><i class="fas fa-compact-disc"></i></div>
          <div class="album-info">
            <h3 class="album-name">${album.name}</h3>
            <div class="album-details"><span>${album.year}</span></div>
            <div class="album-details"><span>${album.songs.length} canción${album.songs.length !== 1 ? "es" : ""}</span></div>
          </div>
        `;

        el.querySelector(".album-options").addEventListener("click", (e) => {
          e.stopPropagation();
          currentAlbumId = album.id;
          currentAlbumName = album.name;
          document.getElementById("edit-album-name").value = album.name;
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
      currentAlbumName = name;
      currentAlbumTitle.textContent = name;
      albumSongsContainer.innerHTML = "";

      songs.forEach((song) => {
        const el = document.createElement("div");
        el.className = "song-item";
        el.innerHTML = `
          <div class="song-info"><strong>${song.title}</strong><p>${song.artist}</p></div>
          <div class="song-actions">
            <span>${formatTime(song.duration)}</span>
            <i class="fas fa-pen-to-square edit-song-btn" data-id="${song.id}"></i>
            <i class="fas fa-trash delete-song-btn" style="color: red" data-id="${song.id}"></i>
          </div>
        `;

        // Selecciona el botón de edición correctamente
        el.querySelector(".edit-song-btn").addEventListener("click", (e) => {
          e.stopPropagation(); // Previene que el evento se propague al contenedor
          const modal = document.getElementById("editCancionModal");

          // Usa querySelector en el modal para encontrar los elementos
          modal.querySelector("#song-name").value = song.title;
          modal.querySelector("#song-genre").value = song.genre; // Corregido de "geren" a "genre" hy3rgffthhtgfvgbtehnyjrmhgtrfergthytgfd
          modal.querySelector("#song-duration").value = song.duration;
          modal.querySelector("#song-url").value = song.url;

          currentSong = song;
          modal.classList.remove("hidden");
        });

        el.querySelector(".delete-song-btn").addEventListener("click", async (e) => {
          e.stopPropagation(); // Previene que el evento se propague al contenedor

          if (confirm(`¿Eliminar la canción "${song.title}" permanentemente?`)) {
            try {
              const res = await window.eliminar_cancion(currentAlbumId, song.id);
              if (res.status !== "ok") throw new Error(res.message);

              alert("Canción eliminada exitosamente");
              window.location.reload();
            } catch (error) {
              alert(error.message);
            }
          }
        });

        albumSongsContainer.appendChild(el);
      });

      document.getElementById("artistSection").classList.add("hidden");
      albumSongsSection.classList.remove("hidden");
    }

    function setupAlbumEvents() {
      // Back to albums
      document.getElementById("backToAlbums").addEventListener("click", () => {
        document.getElementById("albumSongsSection").classList.add("hidden");
        document.getElementById("artistSection").classList.remove("hidden");
      });

      // New album
      document.getElementById("newAlbumBtn").addEventListener("click", () => {
        document.getElementById("newAlbumModal").classList.remove("hidden");
      });

      // Create album
      document.getElementById("createAlbumBtn").addEventListener("click", async () => {
        const name = document.getElementById("album-name").value.trim();
        if (!name) {
          alert("Completa todos los campos");
          return;
        }

        try {
          const res = await window.crear_album(name);
          if (res.status !== "ok")
            throw new Error(res.message);

          alert(res.message);
          document.getElementById("newAlbumModal").classList.add("hidden");
          window.location.reload();
        } catch (error) {
          alert(error.message);
        }
      });

      // Save album
      document.getElementById("saveAlbumBtn").addEventListener("click", async () => {
        const name = document.getElementById("edit-album-name").value.trim();
        if (!name) {
          alert("Completa todos los campos");
          return;
        }

        try {
          const res = await window.actualizar_album(currentAlbumName, name);
          if (res.status !== "ok")
            throw new Error(res.message);

          alert(res.message);
          document.getElementById("editAlbumModal").classList.add("hidden");
          window.location.reload();
        } catch (error) {
          console.warn(error.message);
        }
      });

      // Delete album
      document.getElementById("deleteAlbumBtn").addEventListener("click", async () => {
        if (confirm("¿Eliminar este álbum?")) {
          try {
            const res = await window.eliminar_album(currentAlbumName);

            if (res.status !== "ok") {
              throw new Error(res.message);
            }

            alert(res.message);

            window.location.reload();
          } catch (error) {
            alert(error.message);
          } finally {
            document.getElementById("editAlbumModal").classList.add("hidden");
          }
        }
      });

      document.getElementById("addCancion").addEventListener("click", () => {
        document.getElementById("newSongModal").classList.remove("hidden");
      });

      // NEW SONG MODAL crear cancion
      document.getElementById("newSongForm")?.addEventListener("submit", async (e) => {
        e.preventDefault();

        const title = document.getElementById("song-name").value.trim();
        const genre = document.getElementById("song-genre").value.trim();
        const duration = parseInt(document.getElementById("song-duration").value);
        const url = document.getElementById("song-url").value.trim();

        try {
          const res = await window.crear_cancion(currentAlbumName, title, genre, duration, url);
          if (res.status !== "ok") throw new Error(res.message);

          console.log(res.message);
          document.getElementById("newSongModal").classList.add("hidden");
          window.location.reload();
        } catch (error) {
          alert(error.message);
        }
      });

      // NEW SONG MODAL editar cancion
      document.getElementById("editSongForm")?.addEventListener("submit", async (e) => {
        e.preventDefault();

        // Usar querySelector en el formulario (e.target) para encontrar los elementos
        const title = e.target.querySelector("#song-name").value;
        const genre = e.target.querySelector("#song-genre").value;
        const duration = e.target.querySelector("#song-duration").value;
        const url = e.target.querySelector("#song-url").value;

        try {
          const res = await window.actualizar_cancion(currentAlbumName, currentSong.id, title, genre, duration, url);
          if (res.status !== "ok") throw new Error(res.message);

          console.log(res.message);
          document.getElementById("editCancionModal").classList.add("hidden");

          window.location.reload();
        } catch (error) {
          alert(error.message);
        }
      });

      // Close modals
      document.querySelectorAll(".close-modal").forEach((btn) => {
        btn.addEventListener("click", () => {
          ["plansModal", "newAlbumModal", "editAlbumModal", "newSongModal", "editCancionModal",
            "deleteUserModal", "deleteArtistModal"].forEach((id) => {
              document.getElementById(id).classList.add("hidden");
            });
        });
      });

      ["plansModal", "newAlbumModal", "editAlbumModal", "newSongModal", "editCancionModal",
        "deleteUserModal", "deleteArtistModal"].forEach((id) => {
          document.getElementById(id).addEventListener("click", (e) => {
            if (e.target.id === id) {
              e.target.classList.add("hidden");
            }
          });
        });
    }

    // MAIN INITIALIZATION
    return function () {
      window.obtener_info_usuario()
        .then((res) => {
          if (res.status !== "ok") {
            throw new Error(res?.message || "No se pudo cargar el perfil");
          }

          const userData = res.data;

          // 1. Header section
          setProfileFields(userData);
          setupBecomeArtistButton(userData);
          setupLogout();

          // 2. Profile info section
          setupProfileForm(userData);

          // 3. Password section
          setupPasswordForm();
          setupPasswordToggle();

          // 4. Subscription section
          setSubscriptionInfo(userData);
          document.getElementById("upgradeBtn").addEventListener("click", () => {
            loadPlans(userData);
            document.getElementById("plansModal").classList.remove("hidden");
          });

          // 5. Artist section (if applicable)
          if (userData.isArtist) {
            loadArtistData(userData);
          }

          setupDeleteButtons(userData);
        })
        .catch((err) => {
          console.error("Error al cargar perfil:", err);
        });
    };
  })(),
};