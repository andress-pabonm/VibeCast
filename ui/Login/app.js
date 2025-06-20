/*
 * Manejo del formulario de login
 * Efectos interactivos
 * Validación básica
 */

document.addEventListener("DOMContentLoaded", () => {
  // Elementos del DOM
  const loginForm = document.querySelector(".login-form");
  const passwordInput = document.getElementById("password");
  const togglePassword = document.querySelector(".toggle-password");
  const registerBtn = document.querySelector(".register-btn")

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

  //Validación del formulario
  if (loginForm) {
    loginForm.addEventListener("submit", (e) => {
      e.preventDefault();

      const email = loginForm.querySelector('input[type="email"]');
      const password = loginForm.querySelector('input[type="password"]');

      simulateLogin(email.value, password.value);
    });
  }

  // Efecto hover mejorado para botones
  if (registerBtn) {
      // Efecto hover
      registerBtn.addEventListener('mouseenter', () => {
          registerBtn.style.transform = 'translateY(-3px)';
      });
      
      registerBtn.addEventListener('mouseleave', () => {
          registerBtn.style.transform = 'translateY(0)';
      });
  }
});

/**
 * Simula el proceso de login
 * @param {string} email - Email del usuario
 * @param {string} password - Contraseña del usuario
 */
function simulateLogin(email, password) {
  // console.log("Intentando login con:", { email, password });
  // Aquí iría la lógica real de autenticación

  // Simulación de carga
  const loginBtn = document.querySelector(".login-btn");
  const originalText = loginBtn.innerHTML;

  loginBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Validando...';
  loginBtn.disabled = true;

  // setTimeout(() => {
  //   loginBtn.innerHTML = originalText;
  //   loginBtn.disabled = false;
  //   showSuccess("¡Bienvenido de vuelta!");
  // }, 1500);

  const req = {
    type: "login",
    data: {
      email: email,
      password: password,
    },
  };

  window.enviarMensaje(JSON.stringify(req)).then((res) => {
    loginBtn.innerHTML = originalText;
    loginBtn.disabled = false;
    showSuccess("¡Bienvenido de vuelta!"); //vamos a poner esto?
  });
}

/**
 * Muestra un mensaje de error
 * @param {string} message - Mensaje a mostrar
 */
function showError(message) {
  const errorElement = document.createElement("div");
  errorElement.className = "error-message";
  errorElement.textContent = message;
  errorElement.style.color = "#ff6b6b";
  errorElement.style.marginTop = "1rem";

  const form = document.querySelector(".login-form");
  const existingError = document.querySelector(".error-message");

  if (existingError) {
    existingError.remove();
  }

  form.appendChild(errorElement);

  setTimeout(() => {
    errorElement.style.opacity = "0";
    setTimeout(() => errorElement.remove(), 300);
  }, 3000);
}

/**
 * Muestra un mensaje de éxito
 * @param {string} message - Mensaje a mostrar
 */
function showSuccess(message) {
  const successElement = document.createElement("div");
  successElement.className = "success-message";
  successElement.textContent = message;
  successElement.style.color = "#4cc9f0";
  successElement.style.marginTop = "1rem";

  const form = document.querySelector(".login-form");
  const existingSuccess = document.querySelector(".success-message");

  if (existingSuccess) {
    existingSuccess.remove();
  }

  form.appendChild(successElement);

  setTimeout(() => {
    successElement.style.opacity = "0";
    setTimeout(() => successElement.remove(), 300);
  }, 3000); 
}
