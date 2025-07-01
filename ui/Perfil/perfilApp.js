document.addEventListener('DOMContentLoaded', function() {
    // Verificar si el usuario está logueado
    // window.is_logged_in().then((res) => {
    //     if (res.message === "0") {
    //         window.location.replace("../Login/index.html");
    //     }
    // }); NO SE SI NECESITES ESTO ANDRES XD

    // Elementos del DOM
    const logoutBtn = document.getElementById('logoutBtn');
    const profileForm = document.getElementById('profileForm');
    const passwordForm = document.getElementById('passwordForm');
    const subscriptionInfo = document.getElementById('subscriptionInfo');
    const upgradeBtn = document.getElementById('upgradeBtn');
    const plansModal = document.getElementById('plansModal');
    const closeModals = document.querySelectorAll('.close-modal');
    const plansContainer = document.getElementById('plansContainer');
    const artistSection = document.getElementById('artistSection');
    const albumsContainer = document.getElementById('albumsContainer');
    const newAlbumBtn = document.getElementById('newAlbumBtn');
    const newAlbumModal = document.getElementById('newAlbumModal');
    const createAlbumBtn = document.getElementById('createAlbumBtn');
    const editAlbumModal = document.getElementById('editAlbumModal');
    const saveAlbumBtn = document.getElementById('saveAlbumBtn');
    const deleteAlbumBtn = document.getElementById('deleteAlbumBtn');
    const albumSongsSection = document.getElementById('albumSongsSection');
    const albumSongsContainer = document.getElementById('albumSongsContainer');
    const currentAlbumTitle = document.getElementById('currentAlbumTitle');
    const backToAlbums = document.getElementById('backToAlbums');
    const addAlbumToQueueBtn = document.getElementById('addAlbumToQueueBtn');
    let currentAlbumId = null;

    // Simulación de datos del usuario
    const userData = {
    name: "John Doe",
    username: "@johndoe",
    email: "john.doe@example.com",
    country: "México",
    isArtist: true, //APLICAR LOGICA AQUI ANDRES
    subscription: {
        type: "premium",
        expiration: "2024-12-31", // Fecha futura AQUI TAMBIEN LOGICA
        autoRenewal: true
    }
};

    // Simulación de planes disponibles
    const subscriptionPlans = [
        {
            id: "free",
            name: "Free",
            price: 0,
            period: "Gratis",
            features: [
                "Reproducción con anuncios",
                "Límite de saltos"
            ]
        },
        {
            id: "premium",
            name: "Premium",
            price: 4.99,
            period: "por mes",
            features: [
                "Reproducción sin anuncios",
                "Saltos ilimitados",
            ]
        },
        {
            id: "annual",
            name: "Premium Anual",
            price: 49.99,
            period: "por año",
            features: [
                "Reproducción sin anuncios",
                "Saltos ilimitados",
                "Lo tienes por un año XD"
            ]
    }
    ];

    // Simulación de discos del artista
    let artistAlbums = [
        {
            id: 1,
            name: "Primeros Pasos",
            year: "2020",
            genre: "Rock",
            songs: [
                { id: 1, title: "Canción de inicio", artist: "John Doe", duration: "3:45" },
                { id: 2, title: "El camino", artist: "John Doe", duration: "4:12" },
                { id: 3, title: "Final feliz", artist: "John Doe", duration: "5:23" }
            ]
        },
        {
            id: 2,
            name: "Entre Sombras",
            year: "2022",
            genre: "Pop",
            songs: [
                { id: 1, title: "Luz al amanecer", artist: "John Doe", duration: "3:12" },
                { id: 2, title: "Sombras", artist: "John Doe", duration: "4:45" }
            ]
        }
    ];

    // Cargar datos del perfil
    function loadProfileData() {
        // Información básica
        document.getElementById('profile-name').textContent = userData.name;
        document.getElementById('profile-username').textContent = userData.username;
        document.getElementById('profile-country').innerHTML = `<i class="fas fa-globe"></i> ${userData.country}`;
        document.getElementById('profile-email').innerHTML = `<i class="fas fa-envelope"></i> ${userData.email}`;
        
        // Formulario de perfil
        document.getElementById('edit-name').value = userData.name;
        document.getElementById('edit-username').value = userData.username;
        document.getElementById('edit-country').value = userData.country;
        document.getElementById('edit-email').value = userData.email;
        
        // Cargar información de suscripción
        loadSubscriptionInfo();
        
        // Mostrar sección de artista si corresponde USAR LOGICA
        if (userData.isArtist) {
            artistSection.classList.remove('hidden');
            loadAlbums();
        }
    }

    // Cargar información de suscripción
    function loadSubscriptionInfo() {
        const isPremium = userData.subscription.type === "premium" || userData.subscription.type === "annual";
        // No vale de lo tiempo xd no se porque 
        // const expirationDate = new Date(userData.subscription.expiration);
        // const now = new Date();
        // const timeLeft = expirationDate - now;
        
        let timeLeftText = '';
        let subscriptionStatus = '';
        
        // if (timeLeft <= 0) {
        //     timeLeftText = '<span style="color: var(--error)">Expirado</span>';
        //     subscriptionStatus = 'expirado';
        // } else {
        //     // Calcular días, horas y minutos restantes
        //     const daysLeft = Math.floor(timeLeft / (1000 * 60 * 60 * 24));
        //     const hoursLeft = Math.floor((timeLeft % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
            
        //     if (daysLeft > 0) {
        //         timeLeftText = `Expira en ${daysLeft} día${daysLeft !== 1 ? 's' : ''}`;
        //         if (hoursLeft > 0) {
        //             timeLeftText += ` y ${hoursLeft} hora${hoursLeft !== 1 ? 's' : ''}`;
        //         }
        //     } else {
        //         const minutesLeft = Math.floor((timeLeft % (1000 * 60 * 60)) / (1000 * 60));
        //         timeLeftText = `Expira en ${hoursLeft} hora${hoursLeft !== 1 ? 's' : ''} y ${minutesLeft} minuto${minutesLeft !== 1 ? 's' : ''}`;
        //     }
        // }
        
        subscriptionInfo.innerHTML = `
            <div class="subscription-status ${userData.subscription.type} ${subscriptionStatus}">
                <i class="fas ${isPremium ? 'fa-crown' : 'fa-music'}"></i>
                <span>Plan ${isPremium ? (userData.subscription.type === 'annual' ? 'Premium Anual' : 'Premium') : 'Free'} ${subscriptionStatus ? '(' + subscriptionStatus + ')' : ''}</span>
            </div>
            ${isPremium ? `
                <div class="expiration-date">
                    ${userData.subscription.autoRenewal ? 
                        '<i class="fas fa-sync-alt"></i> Renovación automática' : 
                        `<i class="far fa-clock"></i> ${timeLeftText}`}
                </div>
                <div class="subscription-features">
                    <h4>Beneficios:</h4>
                    <ul>
                        ${subscriptionPlans.find(p => p.id === userData.subscription.type).features.map(f => `
                            <li><i class="fas fa-check"></i> ${f}</li>
                        `).join('')}
                    </ul>
                </div>
            ` : `
                <div class="subscription-features">
                    <h4>Actualiza para obtener:</h4>
                    <ul>
                        ${subscriptionPlans.find(p => p.id === 'premium').features.map(f => `
                            <li><i class="fas fa-check"></i> ${f}</li>
                        `).join('')}
                    </ul>
                </div>
            `}
        `;
        
        upgradeBtn.innerHTML = isPremium ? 
            `<i class="fas fa-sync-alt"></i> ${userData.subscription.type === 'annual' ? 'Renovar plan anual' : 'Renovar plan'}` : 
            `<i class="fas fa-arrow-up"></i> Actualizar a Premium`;
    }
    
    // Cargar discos del artista
    function loadAlbums() {
        albumsContainer.innerHTML = '';
    
        artistAlbums.forEach(album => {
            const albumElement = document.createElement('div');
            albumElement.className = 'album-card';
            albumElement.innerHTML = `
                <div class="album-options" data-id="${album.id}">
                    <i class="fas fa-ellipsis-h"></i>
                </div>
                <div class="album-image">
                    <i class="fas fa-compact-disc"></i>
                </div>
                <div class="album-info">
                    <h3 class="album-name">${album.name}</h3>
                    <div class="album-details">
                        <span>${album.year}</span>
                        <span>${album.genre}</span>
                    </div>
                    <div class="album-details">
                        <span>${album.songs.length} canción${album.songs.length !== 1 ? 'es' : ''}</span>
                    </div>
                </div>
            `;
            
            // Evento para mostrar canciones al hacer clic en el álbum
            albumElement.addEventListener('click', () => showAlbumSongs(album.id, album.name));
            
            // Evento para el botón de opciones
            albumElement.querySelector('.album-options').addEventListener('click', (e) => {
                e.stopPropagation();
                const albumId = parseInt(e.currentTarget.getAttribute('data-id'));
                editAlbum(albumId);
            });
            
            albumsContainer.appendChild(albumElement);
        });
    }

    // Función para mostrar las canciones de un álbum
    function showAlbumSongs(albumId, albumName) {
        currentAlbumId = albumId;
        currentAlbumTitle.textContent = albumName;
        albumSongsContainer.innerHTML = '';
        
        const album = artistAlbums.find(a => a.id === albumId);
        if (!album) return;
        
        album.songs.forEach(song => {
            const songElement = document.createElement('div');
            songElement.className = 'song-item';
            songElement.innerHTML = `
                <div class="song-info">
                    <strong>${song.title}</strong>
                    <p>${song.artist}</p>
                </div>
                <div class="song-actions">
                    <span>${song.duration}</span>
                    <i class="fas fa-play play-btn" title="Reproducir"></i>
                </div>
            `;
            
            // Evento para reproducir la canción
            songElement.querySelector('.play-btn').addEventListener('click', (e) => {
                e.stopPropagation();
                // Aquí iría la lógica para reproducir la canción
                console.log('Reproduciendo:', song.title);
            });
            
            albumSongsContainer.appendChild(songElement);
        });
        
        artistSection.classList.add('hidden');
        albumSongsSection.classList.remove('hidden');
    }

    // Evento para volver a la vista de álbumes
    backToAlbums.addEventListener('click', () => {
        albumSongsSection.classList.add('hidden');
        artistSection.classList.remove('hidden');
    });

    function editAlbum(albumId) {
        const album = artistAlbums.find(a => a.id === albumId);
        if (!album) return;
        
        currentAlbumId = albumId;
        document.getElementById('edit-album-name').value = album.name;
        document.getElementById('edit-album-year').value = album.year;
        editAlbumModal.classList.remove('hidden');
    }

    // Añadir eventos para los botones del modal de edición
    saveAlbumBtn.addEventListener('click', function() {
        const albumName = document.getElementById('edit-album-name').value.trim();
        const albumYear = document.getElementById('edit-album-year').value.trim();
        
        if (!albumName || !albumYear) {
            alert('Por favor completa todos los campos');
            return;
        }
        
        const albumIndex = artistAlbums.findIndex(a => a.id === currentAlbumId);
        if (albumIndex !== -1) {
            artistAlbums[albumIndex].name = albumName;
            artistAlbums[albumIndex].year = albumYear;
            loadAlbums();
            editAlbumModal.classList.add('hidden');
            alert('Álbum actualizado correctamente');
        }
    });

    deleteAlbumBtn.addEventListener('click', function() {
        if (currentAlbumId && confirm('¿Estás seguro de eliminar este álbum?')) {
            artistAlbums = artistAlbums.filter(a => a.id !== currentAlbumId);
            loadAlbums();
            editAlbumModal.classList.add('hidden');
            alert('Álbum eliminado correctamente');
        }
    });

    // Asegurarse de cerrar el modal al hacer clic en la X
    closeModals.forEach(btn => {
        btn.addEventListener('click', function() {
            plansModal.classList.add('hidden');
            newAlbumModal.classList.add('hidden');
            editAlbumModal.classList.add('hidden');
        });
    });

    // Cerrar modal al hacer clic fuera
    editAlbumModal.addEventListener('click', (e) => {
        if (e.target === editAlbumModal) {
            editAlbumModal.classList.add('hidden');
        }
    });

    // Cargar planes disponibles
    function loadPlans() {
        plansContainer.innerHTML = '';
        
        subscriptionPlans.forEach(plan => {
            const isCurrentPlan = plan.id === userData.subscription.type;
            
            const planElement = document.createElement('div');
            planElement.className = `plan-card ${plan.id}`;
            planElement.innerHTML = `
                <h3 class="plan-name">${plan.name}</h3>
                <div class="plan-price">$${plan.price.toFixed(2)}</div>
                <div class="plan-period">${plan.period}</div>
                <div class="plan-features">
                    <ul>
                        ${plan.features.map(feature => `
                            <li><i class="fas fa-check"></i> ${feature}</li>
                        `).join('')}
                    </ul>
                </div>
                <button class="select-plan-btn ${plan.id} ${isCurrentPlan ? 'current' : ''}" 
                        data-plan="${plan.id}">
                    ${isCurrentPlan ? 'Plan actual' : plan.id === 'premium' ? 'Elegir Premium' : 'Continuar con Free'}
                </button>
            `;
            plansContainer.appendChild(planElement);
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
        
        const newName = document.getElementById('edit-name').value;
        const newUsername = document.getElementById('edit-username').value;
        const newCountry = document.getElementById('edit-country').value;
        const newEmail = document.getElementById('edit-email').value;
        
        // Validar nombre de usuario
        if (!newUsername.startsWith('@')) {
            alert('El nombre de usuario debe comenzar con @');
            return;
        }
        
        // Actualizar datos
        userData.name = newName;
        userData.username = newUsername;
        userData.country = newCountry;
        userData.email = newEmail;
        
        // Actualizar la vista
        document.getElementById('profile-name').textContent = newName;
        document.getElementById('profile-username').textContent = newUsername;
        document.getElementById('profile-country').innerHTML = `<i class="fas fa-globe"></i> ${newCountry}`;
        document.getElementById('profile-email').innerHTML = `<i class="fas fa-envelope"></i> ${newEmail}`;
        
        alert('Perfil actualizado correctamente');
    });

    // Evento para cambiar contraseña
    passwordForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        const newPassword = document.getElementById('new-password').value;
        const confirmPassword = document.getElementById('confirm-new-password').value;
        
        if (newPassword !== confirmPassword) {
            alert('Las contraseñas no coinciden');
            return;
        }
        
        if (newPassword.length < 8) {
            alert('La contraseña debe tener al menos 8 caracteres');
            return;
        }
         
        // Validar fortaleza de contraseña supongo que esto en el c
        // const hasUpperCase = /[A-Z]/.test(newPassword);
        // const hasLowerCase = /[a-z]/.test(newPassword);
        // const hasNumbers = /\d/.test(newPassword);
        // const hasSpecialChars = /[!@#$%^&*(),.?":{}|<>]/.test(newPassword);
        
        if (!hasUpperCase || !hasLowerCase || !hasNumbers || !hasSpecialChars) {
            alert('La contraseña debe contener al menos una mayúscula, una minúscula, un número y un carácter especial');
            return;
        }
        
        // Simular cambio de contraseña
        alert('Contraseña cambiada correctamente');
        passwordForm.reset();
    });

    // Evento para mostrar/ocultar contraseña
    document.querySelectorAll('.toggle-password').forEach(btn => {
        btn.addEventListener('click', function() {
            const input = this.closest('.input-with-icon').querySelector('input');
            const type = input.getAttribute('type') === 'password' ? 'text' : 'password';
            input.setAttribute('type', type);
            
            this.classList.toggle('fa-eye');
            this.classList.toggle('fa-eye-slash');
        });
    });

    // Evento para abrir modal de planes
    upgradeBtn.addEventListener('click', function() {
        loadPlans();
        plansModal.classList.remove('hidden');
    });

    // Evento para crear nuevo disco
    newAlbumBtn.addEventListener('click', function() {
        newAlbumModal.classList.remove('hidden');
    });

    // Evento para confirmar creación de disco
    createAlbumBtn.addEventListener('click', function() {
        const albumName = document.getElementById('album-name').value.trim();
        const albumYear = document.getElementById('album-year').value.trim();
        const albumGenre = document.getElementById('album-genre').value.trim();
        
        if (!albumName || !albumYear || !albumGenre) {
            alert('Por favor completa todos los campos');
            return;
        }
        
        // Crear nuevo disco
        const newAlbum = {
            id: Math.max(...artistAlbums.map(a => a.id), 0) + 1,
            name: albumName,
            year: albumYear,
            genre: albumGenre,
            songs: 0
        };
        
        artistAlbums.push(newAlbum);
        loadAlbums();
        
        // Limpiar y cerrar modal
        document.getElementById('album-name').value = '';
        document.getElementById('album-year').value = '';
        document.getElementById('album-genre').value = '';
        newAlbumModal.classList.add('hidden');
        
        alert(`Disco "${albumName}" creado correctamente`);
    });

    // Cerrar modals
    closeModals.forEach(btn => {
        btn.addEventListener('click', function() {
            plansModal.classList.add('hidden');
            newAlbumModal.classList.add('hidden');
        });
    });


    // Cerrar modals al hacer clic fuera
    [plansModal, newAlbumModal].forEach(modal => {
        modal.addEventListener('click', (e) => {
            if (e.target === modal) {
                modal.classList.add('hidden');
            }
        });
    });


    // Evento para seleccionar plan
    plansContainer.addEventListener('click', function(e) {
        if (e.target.classList.contains('select-plan-btn')) {
            const planId = e.target.getAttribute('data-plan');
            
            if (e.target.classList.contains('current')) return;
            
            if (confirm(`¿Estás seguro de que quieres cambiar al plan ${planId === 'premium' ? 'Premium' : planId === 'annual' ? 'Premium Anual' : 'Free'}?`)) {
                userData.subscription.type = planId;
                
                if (planId === 'premium' || planId === 'annual') {
                    const expiration = new Date();
                    if (planId === 'annual') {
                        expiration.setFullYear(expiration.getFullYear() + 1);
                    } else {
                        expiration.setMonth(expiration.getMonth() + 1);
                    }
                    userData.subscription.expiration = expiration.toISOString().split('T')[0];
                    userData.subscription.autoRenewal = true;
                } else {
                    userData.subscription.expiration = null;
                    userData.subscription.autoRenewal = false;
                }
                
                loadSubscriptionInfo();
                plansModal.classList.add('hidden');
                alert(`¡Felicidades! Ahora tienes el plan ${planId === 'premium' ? 'Premium' : planId === 'annual' ? 'Premium Anual' : 'Free'}`);
            }
        }
    });

    // Inicializar
    loadProfileData();
});