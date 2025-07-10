window.views = window.views || {};

window.views.amigos = {
  title: `<i class="fas fa-user-friends"></i>Amigos`,
  html: `
    <header class="main-header">
        <div class="search-bar">
            <i class="fas fa-search search-icon"></i>
            <input type="text" id="friendSearch" placeholder="Buscar amigos..." class="search-input">
        </div>
        <button class="add-friend-btn" id="addFriendBtn">
            <i class="fas fa-user-plus"></i> Añadir amigo
        </button>
    </header>

    <div class="friends-container">
        <!-- Sección de Amigos -->
        <section class="friends-section">
            <h2 class="section-title"><i class="fas fa-users"></i> Tus Amigos</h2>
            <div class="friends-list" id="friendsList">
                <!-- Amigos se cargarán dinámicamente con esta estructura -->
                <div class="friend-card template" style="display: none;">
                    <div class="friend-avatar">
                        <i class="fas fa-user"></i>
                    </div>
                    <div class="friend-name"></div>
                    <div class="friend-actions">
                        <button class="action-btn remove">
                            <i class="fas fa-user-minus"></i>
                        </button>
                    </div>
                </div>
            </div>
        </section>
    </div>

    <!-- Modal para añadir amigo -->
    <div class="modal hidden" id="addFriendModal">
        <div class="modal-content">
            <span class="close-modal">&times;</span>
            <h3>Añadir nuevo amigo</h3>
            <input type="text" id="friendUsername" placeholder="Nombre de usuario">
            <button id="sendRequestBtn">Enviar solicitud</button>
        </div>
    </div>
  `,
  init: function () {
    function init() {
      // Elementos del DOM
      const friendsList = document.getElementById("friendsList");
      const friendTemplate = friendsList.querySelector(".template");
      const addFriendBtn = document.getElementById("addFriendBtn");
      const addFriendModal = document.getElementById("addFriendModal");
      const closeModal = document.querySelector(".close-modal");
      const sendRequestBtn = document.getElementById("sendRequestBtn");
      const friendSearch = document.getElementById("friendSearch");

      // Datos de ejemplo
      let friendsData = [
        { id: 1, name: "María García" },
        { id: 2, name: "Carlos López" },
        { id: 3, name: "Ana Martínez" },
        { id: 4, name: "David Fernández" },
      ];

      // Cargar amigos
      function loadFriends() {
        // Limpiar lista (excepto la plantilla)
        document
          .querySelectorAll(".friend-card:not(.template)")
          .forEach((el) => el.remove());

        friendsData.forEach((friend) => {
          const friendElement = createFriendElement(friend);
          friendsList.appendChild(friendElement);
        });
      }

      // Crear elemento de amigo desde plantilla
      function createFriendElement(friend) {
        const friendCard = friendTemplate.cloneNode(true);
        friendCard.classList.remove("template");
        friendCard.style.display = "flex";

        friendCard.querySelector(".friend-name").textContent = friend.name;

        // Configurar evento de eliminación
        friendCard.querySelector(".remove").addEventListener("click", () => {
          if (confirm(`¿Eliminar a ${friend.name} de tus amigos?`)) {
            friendsData = friendsData.filter((f) => f.id !== friend.id);
            loadFriends();
          }
        });

        return friendCard;
      }

      // Búsqueda de amigos
      friendSearch.addEventListener("input", function () {
        const searchTerm = this.value.toLowerCase();
        const filteredFriends = friendsData.filter((friend) =>
          friend.name.toLowerCase().includes(searchTerm)
        );

        // Limpiar lista (excepto la plantilla)
        document
          .querySelectorAll(".friend-card:not(.template)")
          .forEach((el) => el.remove());

        filteredFriends.forEach((friend) => {
          friendsList.appendChild(createFriendElement(friend));
        });
      });

      // Resto del código se mantiene igual...
      addFriendBtn.addEventListener("click", () => {
        addFriendModal.classList.remove("hidden");
      });

      closeModal.addEventListener("click", () => {
        addFriendModal.classList.add("hidden");
      });

      sendRequestBtn.addEventListener("click", () => {
        const username = document.getElementById("friendUsername").value.trim();
        if (username) {
          // Añadir nuevo amigo (ejemplo)
          const newFriend = {
            id: friendsData.length + 1,
            name: username,
          };
          friendsData.push(newFriend);
          loadFriends();

          alert(`Solicitud enviada a ${username}`);
          document.getElementById("friendUsername").value = "";
          addFriendModal.classList.add("hidden");
        }
      });

      addFriendModal.addEventListener("click", (e) => {
        if (e.target === addFriendModal) {
          addFriendModal.classList.add("hidden");
        }
      });

      // Inicializar
      loadFriends();
    }

    init();
  },
};
