document.getElementById('registerForm').addEventListener('submit', function(event) {
    event.preventDefault();

    const newUsername = document.getElementById('newUsername').value;
    const newPassword = document.getElementById('newPassword').value;
    const confirmPassword = document.getElementById('confirmPassword').value;

    // Verifica si las contraseñas coinciden
    if (newPassword !== confirmPassword) {
        document.getElementById('registerResponse').innerText = 'Las contraseñas no coinciden';
        return;
    }

    // Simula una llamada a la lógica de C para registrar al usuario
    registerUser(`register:${newUsername}:${newPassword}`);
});

function registerUser(message) {
    // Asumiendo que has configurado WebView para exponer una función de C
    window.cef.query({ request: message }, (response) => {
        document.getElementById('registerResponse').innerText = response;
    });
}