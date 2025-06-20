/*
 * Manejo del registro
 * Efectos interactivos
 * Validación básica
 */

document.addEventListener('DOMContentLoaded', () => {
    // ========== SELECTORES ==========
    const registerBtn = document.querySelector('.register-btn');

    // Validación específica para contraseña Faltaria validar que las dos contraseñas son iguales
    // if (input.type === 'password' && input.value.length < 6) {
    //     parent.classList.add('error');
    //     errorMessage.textContent = 'La contraseña debe tener al menos 6 caracteres';
    //     errorMessage.style.display = 'block';
    //     return;
    // }
    
    // function isValidEmail(email) {
    //     return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email);
    // }
    
    // ========== MOSTRAR/OCULTAR CONTRASEÑA ==========
    document.querySelectorAll('.toggle-password').forEach(btn => {
        btn.addEventListener('click', function() {
            // Encontrar el input asociado a este botón
            const input = this.closest('.input-group').querySelector('input');

            // Cambiar el tipo de input
            const type = input.getAttribute('type') === 'password' ? 'text' : 'password';
            input.setAttribute('type', type);

            // Cambiar el icono
            this.classList.toggle('fa-eye');
            this.classList.toggle('fa-eye-slash');
        });
    });
    
    // ========== EFECTOS DE BOTÓN ==========
    if (registerBtn) {
        // Efecto hover
        registerBtn.addEventListener('mouseenter', () => {
            registerBtn.style.transform = 'translateY(-3px)';
        });
        
        registerBtn.addEventListener('mouseleave', () => {
            registerBtn.style.transform = 'translateY(0)';
        });
    }
    
    // ========== ENVÍO DE FORMULARIO ==========
    //Aqui iria la logica de envio y validacion del formulario       
    // Animación de carga
    // registerBtn.innerHTML = '<i class="fas fa-spinner fa-spin btn-icon"></i> Creando tu cuenta...';
    // registerBtn.disabled = true;
    
    // Simular envío al servidor
//     setTimeout(() => {
//         registerBtn.innerHTML = '<i class="fas fa-check btn-icon"></i> ¡Registro exitoso!';
//         registerBtn.style.background = 'var(--spider-azul)';
//         // Redirigir después de 2 segundos
//         setTimeout(() => {
//             window.location.href = '../Menu/menu.html'; // Cambia por tu URL
//         }, 2000);
//     }, 2000);
// });
});