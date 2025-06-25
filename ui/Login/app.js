/*
 * Manejo del formulario de login
 * Efectos interactivos
 * Validación básica
 */

document.addEventListener("DOMContentLoaded", () => {
  window.is_logged_in().then((res) => {
    console.log("islogged_in(): ", res);
    if (res.message === "1") {
      window.location.replace("../Menu/menu.html");
    }
  });

  // Elementos del DOM
  const loginForm = document.querySelector(".login-form");
  const registerBtn = document.querySelector(".register-btn");

  document.querySelectorAll(".toggle-password").forEach((btn) => {
    btn.addEventListener("click", function () {
      // Encontrar el input asociado a este botón
      const input = this.closest(".input-group").querySelector("input");

      // Cambiar el tipo de input
      const type =
        input.getAttribute("type") === "password" ? "text" : "password";
      input.setAttribute("type", type);

      // Cambiar el icono
      this.classList.toggle("fa-eye");
      this.classList.toggle("fa-eye-slash");
    });
  });

  // Envio del formulario
  if (loginForm) {
    loginForm.addEventListener("submit", (e) => {
      e.preventDefault();

      const username = loginForm.querySelector("#username");
      const password = loginForm.querySelector("#password");

      simulateLogin(username.value, password.value);
    });
  }

  // Efecto hover mejorado para botones
  if (registerBtn) {
    // Efecto hover
    registerBtn.addEventListener("mouseenter", () => {
      registerBtn.style.transform = "translateY(-3px)";
    });

    registerBtn.addEventListener("mouseleave", () => {
      registerBtn.style.transform = "translateY(0)";
    });
  }
});

/**
 * Simula el proceso de login
 * @param {string} username - Email del usuario
 * @param {string} password - Contraseña del usuario
 */
function simulateLogin(username, password) {
  // Simulación de carga
  const loginBtn = document.querySelector(".login-btn");
  const originalText = loginBtn.innerHTML;

  loginBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Validando...';
  loginBtn.disabled = true;

  window.iniciar_sesion(username, password).then((res) => {
    loginBtn.innerHTML = originalText;
    loginBtn.disabled = false;
    console.log("iniciar_sesion(): ", res);
    if (res.message === "1") {
      showSuccess("¡Bienvenido de vuelta!");
      setTimeout(() => {
        window.location.reload(); // Refresca la página
      }, 500);
    } else {
      showError("Usuario o contraseña incorrectos.");
    }
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
