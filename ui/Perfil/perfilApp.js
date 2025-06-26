document.addEventListener('DOMContentLoaded', function() {
    // Verificar si el usuario está logueado
    window.is_logged_in().then((res) => {
        if (res.message === "0") {
            window.location.replace("../Login/index.html");
        }
    });

    // Elementos del DOM
    const logoutBtn = document.getElementById('logoutBtn');
    const profileForm = document.getElementById('profileForm');
    const passwordForm = document.getElementById('passwordForm');
    const artistSection = document.getElementById('artistSection');
    const albumsContainer = document.getElementById('albumsContainer');
    const newAlbumBtn = document.getElementById('newAlbumBtn');
    const newAlbumModal = document.getElementById('newAlbumModal');
    const closeModal = document.querySelector('.close-modal');
    const createAlbumBtn = document.getElementById('createAlbumBtn');

    // Simulación de datos del usuario (en un caso real, estos datos vendrían de la API)
    const userData = {
        name: "John Doe",
        username: "@johndoe",
        email: "john.doe@example.com",
        country: "México",
        bio: "Amante de la música desde pequeño. Mis géneros favoritos son el rock y el jazz.",
        isArtist: true
    };

    // Simulación de álbumes del artista
    const artistAlbums = [
        { id: 1, name: "Primeros Pasos", year: "2020", songs: 8 },
        { id: 2, name: "Entre Sombras", year: "2022", songs: 12 },
        { id: 3, name: "Luz al Final", year: "2023", songs: 10 }
    ];

    // Cargar datos del perfil
    function loadProfileData() {
        document.getElementById('profile-name').textContent = userData.name;
        document.getElementById('profile-username').textContent = userData.username;
        document.getElementById('profile-country').innerHTML = `<i class="fas fa-globe"></i> ${userData.country}`;
        
        // Rellenar formulario
        document.getElementById('edit-name').value = userData.name;
        document.getElementById('edit-country').value = userData.country;
        document.getElementById('edit-email').value = userData.email;
        document.getElementById('edit-bio').value = userData.bio || '';
        
        // Mostrar sección de artista si corresponde
        if (userData.isArtist) {
            artistSection.classList.remove('hidden');
            loadAlbums();
        }
    }

    // Cargar álbumes del artista
    function loadAlbums() {
        albumsContainer.innerHTML = '';
        
        artistAlbums.forEach(album => {
            const albumElement = document.createElement('div');
            albumElement.className = 'album-card';
            albumElement.innerHTML = `
                <div class="album-image">
                    <i class="fas fa-compact-disc"></i>
                </div>
                <div class="album-info">
                    <h3 class="album-name">${album.name}</h3>
                    <p class="album-year">${album.year} · ${album.songs} canciones</p>
                </div>
            `;
            albumsContainer.appendChild(albumElement);
        });
    }

    // Evento para cerrar sesión
    logoutBtn.addEventListener('click', function() {
        window.cerrar_sesion().then((res) => {
            if (res.message === "1") {
                window.location.replace("../Login/index.html");
            }
        });
    });

    // Evento para guardar cambios del perfil
    profileForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        // Simular guardado de cambios
        const newName = document.getElementById('edit-name').value;
        const newCountry = document.getElementById('edit-country').value;
        const newEmail = document.getElementById('edit-email').value;
        const newBio = document.getElementById('edit-bio').value;
        
        // Actualizar datos (en un caso real, aquí se haría una petición a la API)
        userData.name = newName;
        userData.country = newCountry;
        userData.email = newEmail;
        userData.bio = newBio;
        
        // Actualizar la vista
        document.getElementById('profile-name').textContent = newName;
        document.getElementById('profile-country').innerHTML = `<i class="fas fa-globe"></i> ${newCountry}`;
        
        // Mostrar feedback
        alert('Perfil actualizado correctamente');
    });

    // Evento para cambiar contraseña
    passwordForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        const currentPassword = document.getElementById('current-password').value;
        const newPassword = document.getElementById('new-password').value;
        const confirmPassword = document.getElementById('confirm-new-password').value;
        
        if (newPassword !== confirmPassword) {
            alert('Las contraseñas no coinciden');
            return;
        }
        
        // Simular cambio de contraseña (en un caso real, aquí se haría una petición a la API)
        alert('Contraseña cambiada correctamente');
        passwordForm.reset();
    });

    // Eventos para mostrar/ocultar contraseña
    document.querySelectorAll('.toggle-password').forEach(btn => {
        btn.addEventListener('click', function() {
            const input = this.closest('.input-with-icon').querySelector('input');
            const type = input.getAttribute('type') === 'password' ? 'text' : 'password';
            input.setAttribute('type', type);
            
            this.classList.toggle('fa-eye');
            this.classList.toggle('fa-eye-slash');
        });
    });

    // Eventos para la sección de artista
    if (newAlbumBtn) {
        newAlbumBtn.addEventListener('click', () => {
            newAlbumModal.classList.remove('hidden');
        });
    }

    if (closeModal) {
        closeModal.addEventListener('click', () => {
            newAlbumModal.classList.add('hidden');
        });
    }

    if (createAlbumBtn) {
        createAlbumBtn.addEventListener('click', () => {
            const albumName = document.getElementById('albumNameInput').value.trim();
            const albumYear = document.getElementById('albumYearInput').value.trim();
            
            if (albumName && albumYear) {
                // Simular creación de álbum
                const newId = Math.max(...artistAlbums.map(a => a.id), 0) + 1;
                artistAlbums.push({
                    id: newId,
                    name: albumName,
                    year: albumYear,
                    songs: 0
                });
                
                // Limpiar y cerrar modal
                document.getElementById('albumNameInput').value = '';
                document.getElementById('albumYearInput').value = '';
                newAlbumModal.classList.add('hidden');
                
                // Recargar lista de álbumes
                loadAlbums();
            }
        });
    }

    // Cerrar modal al hacer clic fuera
    newAlbumModal.addEventListener('click', (e) => {
        if (e.target === newAlbumModal) {
            newAlbumModal.classList.add('hidden');
        }
    });

    // Inicializar
    loadProfileData();
});