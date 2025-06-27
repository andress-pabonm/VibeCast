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
            songs: 8
        },
        {
            id: 2,
            name: "Entre Sombras",
            year: "2022",
            genre: "Pop",
            songs: 12
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
        
        // Cargar información de suscripción
        loadSubscriptionInfo();
        
        // Mostrar sección de artista si corresponde
        if (userData.isArtist) {
            artistSection.classList.remove('hidden');
            loadAlbums();
        }
    }

    // Cargar información de suscripción
    function loadSubscriptionInfo() {
        const isPremium = userData.subscription.type === "premium" || userData.subscription.type === "annual";
        const expirationDate = new Date(userData.subscription.expiration);
        const now = new Date();
        const timeLeft = expirationDate - now;
        
        let timeLeftText = '';
        let subscriptionStatus = '';
        
        if (timeLeft <= 0) {
            timeLeftText = '<span style="color: var(--error)">Expirado</span>';
            subscriptionStatus = 'expirado';
        } else {
            // Calcular días, horas y minutos restantes
            const daysLeft = Math.floor(timeLeft / (1000 * 60 * 60 * 24));
            const hoursLeft = Math.floor((timeLeft % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
            
            if (daysLeft > 0) {
                timeLeftText = `Expira en ${daysLeft} día${daysLeft !== 1 ? 's' : ''}`;
                if (hoursLeft > 0) {
                    timeLeftText += ` y ${hoursLeft} hora${hoursLeft !== 1 ? 's' : ''}`;
                }
            } else {
                const minutesLeft = Math.floor((timeLeft % (1000 * 60 * 60)) / (1000 * 60));
                timeLeftText = `Expira en ${hoursLeft} hora${hoursLeft !== 1 ? 's' : ''} y ${minutesLeft} minuto${minutesLeft !== 1 ? 's' : ''}`;
            }
        }
        
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
                        <span>${album.songs} canción${album.songs !== 1 ? 'es' : ''}</span>
                    </div>
                </div>
            `;
            albumsContainer.appendChild(albumElement);
        });
    }

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
        
        // Validar nombre de usuario
        if (!newUsername.startsWith('@')) {
            alert('El nombre de usuario debe comenzar con @');
            return;
        }
        
        // Actualizar datos
        userData.name = newName;
        userData.username = newUsername;
        userData.country = newCountry;
        
        // Actualizar la vista
        document.getElementById('profile-name').textContent = newName;
        document.getElementById('profile-username').textContent = newUsername;
        document.getElementById('profile-country').innerHTML = `<i class="fas fa-globe"></i> ${newCountry}`;
        
        alert('Perfil actualizado correctamente');
    });

    // Evento para cambiar contraseña
    passwordForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        const currentPassword = document.getElementById('current-pasassword').value;
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
        
        // Validar fortaleza de contraseña
        const hasUpperCase = /[A-Z]/.test(newPassword);
        const hasLowerCase = /[a-z]/.test(newPassword);
        const hasNumbers = /\d/.test(newPassword);
        const hasSpecialChars = /[!@#$%^&*(),.?":{}|<>]/.test(newPassword);
        
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

    // Cerrar modales
    closeModals.forEach(btn => {
        btn.addEventListener('click', function() {
            plansModal.classList.add('hidden');
            newAlbumModal.classList.add('hidden');
        });
    });

    // Cerrar modales al hacer clic fuera
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

    // Al final de perfilApp.js, después de loadProfileData();
    if (userData.subscription.type !== 'free') {
        setInterval(loadSubscriptionInfo, 60000); // Actualiza cada minuto
    }
});