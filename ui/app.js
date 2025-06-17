document.getElementById('loginForm').addEventListener('submit', function(event) {
    event.preventDefault();

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    // Simula una llamada a la lógica de C
    sendMessageToC(`login:${username}:${password}`);
});

function sendMessageToC(message) {
    // Asumiendo que has configurado WebView para exponer una función de C
    window.cef.query({ request: message }, (response) => {
        document.getElementById('respuesta').innerText = response;
    });
}
