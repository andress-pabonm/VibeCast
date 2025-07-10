document.addEventListener('DOMContentLoaded', function() {
    // Elementos del DOM
    const friendsList = document.getElementById('friendsList');
    const friendTemplate = friendsList.querySelector('.template');
    const addFriendBtn = document.getElementById('addFriendBtn');
    const addFriendModal = document.getElementById('addFriendModal');
    const closeModal = document.querySelector('.close-modal');
    const sendRequestBtn = document.getElementById('sendRequestBtn');
    const friendSearch = document.getElementById('friendSearch');
    
    // Datos de ejemplo
    let friendsData = [
        { id: 1, name: "María García" },
        { id: 2, name: "Carlos López" },
        { id: 3, name: "Ana Martínez" },
        { id: 4, name: "David Fernández" }
    ];
    
    // Cargar amigos
    function loadFriends() {
        // Limpiar lista (excepto la plantilla)
        document.querySelectorAll('.friend-card:not(.template)').forEach(el => el.remove());
        
        friendsData.forEach(friend => {
            const friendElement = createFriendElement(friend);
            friendsList.appendChild(friendElement);
        });
    }
    
    // Crear elemento de amigo desde plantilla
    function createFriendElement(friend) {
        const friendCard = friendTemplate.cloneNode(true);
        friendCard.classList.remove('template');
        friendCard.style.display = 'flex';
        
        friendCard.querySelector('.friend-name').textContent = friend.name;
        
        // Configurar evento de eliminación
        friendCard.querySelector('.remove').addEventListener('click', () => {
            if (confirm(`¿Eliminar a ${friend.name} de tus amigos?`)) {
                friendsData = friendsData.filter(f => f.id !== friend.id);
                loadFriends();
            }
        });
        
        return friendCard;
    }
    
    // Búsqueda de amigos
    friendSearch.addEventListener('input', function() {
        const searchTerm = this.value.toLowerCase();
        const filteredFriends = friendsData.filter(friend => 
            friend.name.toLowerCase().includes(searchTerm));
        
        // Limpiar lista (excepto la plantilla)
        document.querySelectorAll('.friend-card:not(.template)').forEach(el => el.remove());
        
        filteredFriends.forEach(friend => {
            friendsList.appendChild(createFriendElement(friend));
        });
    });
    
    // Resto del código se mantiene igual...
    addFriendBtn.addEventListener('click', () => {
        addFriendModal.classList.remove('hidden');
    });
    
    closeModal.addEventListener('click', () => {
        addFriendModal.classList.add('hidden');
    });
    
    sendRequestBtn.addEventListener('click', () => {
        const username = document.getElementById('friendUsername').value.trim();
        if (username) {
            // Añadir nuevo amigo (ejemplo)
            const newFriend = {
                id: friendsData.length + 1,
                name: username
            };
            friendsData.push(newFriend);
            loadFriends();
            
            alert(`Solicitud enviada a ${username}`);
            document.getElementById('friendUsername').value = '';
            addFriendModal.classList.add('hidden');
        }
    });
    
    addFriendModal.addEventListener('click', (e) => {
        if (e.target === addFriendModal) {
            addFriendModal.classList.add('hidden');
        }
    });
    
    // Inicializar
    loadFriends();
});