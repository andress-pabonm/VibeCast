document.addEventListener('DOMContentLoaded', function() {
    // Elementos del DOM
    const friendsList = document.getElementById('friendsList');
    const requestsList = document.getElementById('requestsList');
    const requestsCount = document.getElementById('requestsCount');
    const addFriendBtn = document.getElementById('addFriendBtn');
    const addFriendModal = document.getElementById('addFriendModal');
    const closeModal = document.querySelector('.close-modal');
    const sendRequestBtn = document.getElementById('sendRequestBtn');
    const friendSearch = document.getElementById('friendSearch');
    
    // Datos de ejemplo (simulando una base de datos)
    let friendsData = [
        { id: 1, name: "María García", status: "En línea", avatar: null, lastSeen: "" },
        { id: 2, name: "Carlos López", status: "Escuchando: Bohemian Rhapsody", avatar: null, lastSeen: "" },
        { id: 3, name: "Ana Martínez", status: "Recientemente activo", avatar: null, lastSeen: "5 min" },
        { id: 4, name: "David Fernández", status: "Offline", avatar: null, lastSeen: "2 h" }
    ];
    
    let friendRequests = [
        { id: 5, name: "Laura Sánchez", message: "Quiere ser tu amigo", avatar: null },
        { id: 6, name: "Javier Ruiz", message: "Quiere ser tu amigo", avatar: null }
    ];
    
    // Cargar amigos y solicitudes
    function loadFriends() {
        friendsList.innerHTML = '';
        friendsData.forEach(friend => {
            const friendElement = createFriendElement(friend);
            friendsList.appendChild(friendElement);
        });
    }
    
    function loadRequests() {
        requestsList.innerHTML = '';
        requestsCount.textContent = friendRequests.length;
        
        if (friendRequests.length === 0) {
            requestsList.innerHTML = '<p class="no-requests">No tienes solicitudes pendientes</p>';
            return;
        }
        
        friendRequests.forEach(request => {
            const requestElement = createRequestElement(request);
            requestsList.appendChild(requestElement);
        });
    }
    
    // Crear elementos HTML
    function createFriendElement(friend) {
        const friendCard = document.createElement('div');
        friendCard.className = 'friend-card';
        
        friendCard.innerHTML = `
            <div class="friend-avatar">
                ${friend.avatar ? 
                    `<img src="${friend.avatar}" alt="${friend.name}">` : 
                    `<i class="fas fa-user"></i>`}
            </div>
            <div class="friend-info">
                <div class="friend-name">${friend.name}</div>
                <div class="friend-status">
                    <i class="fas fa-circle ${friend.status === 'En línea' ? 'online' : 'offline'}"></i>
                    ${friend.status} ${friend.lastSeen ? `· ${friend.lastSeen}` : ''}
                </div>
            </div>
            <div class="friend-actions">
                <button class="action-btn message" title="Enviar mensaje">
                    <i class="fas fa-comment-dots"></i>
                </button>
                <button class="action-btn remove" title="Eliminar amigo">
                    <i class="fas fa-user-minus"></i>
                </button>
            </div>
        `;
        
        // Event listeners para los botones
        friendCard.querySelector('.message').addEventListener('click', () => {
            // Aquí iría la lógica para enviar mensaje
            console.log(`Mensaje a ${friend.name}`);
        });
        
        friendCard.querySelector('.remove').addEventListener('click', () => {
            if (confirm(`¿Seguro que quieres eliminar a ${friend.name} de tus amigos?`)) {
                friendsData = friendsData.filter(f => f.id !== friend.id);
                loadFriends();
            }
        });
        
        return friendCard;
    }
    
    function createRequestElement(request) {
        const requestCard = document.createElement('div');
        requestCard.className = 'request-card';
        
        requestCard.innerHTML = `
            <div class="request-avatar">
                ${request.avatar ? 
                    `<img src="${request.avatar}" alt="${request.name}">` : 
                    `<i class="fas fa-user"></i>`}
            </div>
            <div class="request-info">
                <div class="request-name">${request.name}</div>
                <div class="request-message">
                    <i class="fas fa-user-plus"></i>
                    ${request.message}
                </div>
            </div>
            <div class="request-actions">
                <button class="action-btn accept" title="Aceptar">
                    <i class="fas fa-check"></i>
                </button>
                <button class="action-btn reject" title="Rechazar">
                    <i class="fas fa-times"></i>
                </button>
            </div>
        `;
        
        // Event listeners para los botones
        requestCard.querySelector('.accept').addEventListener('click', () => {
            friendsData.push({
                id: request.id,
                name: request.name,
                status: "Nuevo amigo",
                avatar: request.avatar,
                lastSeen: ""
            });
            
            friendRequests = friendRequests.filter(r => r.id !== request.id);
            loadFriends();
            loadRequests();
        });
        
        requestCard.querySelector('.reject').addEventListener('click', () => {
            friendRequests = friendRequests.filter(r => r.id !== request.id);
            loadRequests();
        });
        
        return requestCard;
    }
    
    // Funcionalidad de búsqueda
    friendSearch.addEventListener('input', function() {
        const searchTerm = this.value.toLowerCase();
        const filteredFriends = friendsData.filter(friend => 
            friend.name.toLowerCase().includes(searchTerm)
            || friend.status.toLowerCase().includes(searchTerm));
        
        friendsList.innerHTML = '';
        filteredFriends.forEach(friend => {
            const friendElement = createFriendElement(friend);
            friendsList.appendChild(friendElement);
        });
    });
    
    // Funcionalidad del modal
    addFriendBtn.addEventListener('click', () => {
        addFriendModal.classList.remove('hidden');
    });
    
    closeModal.addEventListener('click', () => {
        addFriendModal.classList.add('hidden');
    });
    
    sendRequestBtn.addEventListener('click', () => {
        const username = document.getElementById('friendUsername').value.trim();
        
        if (username) {
            // Aquí iría la lógica para enviar la solicitud
            alert(`Solicitud enviada a ${username}`);
            document.getElementById('friendUsername').value = '';
            addFriendModal.classList.add('hidden');
        } else {
            alert('Por favor ingresa un nombre de usuario');
        }
    });
    
    // Cerrar modal al hacer clic fuera del contenido
    addFriendModal.addEventListener('click', (e) => {
        if (e.target === addFriendModal) {
            addFriendModal.classList.add('hidden');
        }
    });
    
    // Inicializar la página
    loadFriends();
    loadRequests();
});