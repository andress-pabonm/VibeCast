document.addEventListener("DOMContentLoaded", () => {
  checkUserLoggedIn();

  setupPasswordToggle();
  setupLoginForm();
  setupRegisterButtonHover();
});

/**
 * Verifica si el usuario ya está logueado y redirige si es así.
 */
async function checkUserLoggedIn() {
  try {
    const res = await window.is_logged_in();
    console.log("is_logged_in():", res);

    if (res.status === "ok" && res.type === "boolean" && res.data === true) {
      window.location.replace("../Menu/menu.html");
    }
  } catch (err) {
    console.error("Error al verificar sesión:", err);
  }
}

/**
 * Configura el botón para mostrar/ocultar la contraseña.
 */
function setupPasswordToggle() {
  document.querySelectorAll(".toggle-password").forEach((btn) => {
    btn.addEventListener("click", () => {
      const input = btn.closest(".input-group").querySelector("input");
      const newType = input.type === "password" ? "text" : "password";
      input.type = newType;
      btn.classList.toggle("fa-eye");
      btn.classList.toggle("fa-eye-slash");
    });
  });
}

/**
 * Configura el formulario de login para manejar el envío.
 */
function setupLoginForm() {
  const loginForm = document.querySelector(".login-form");
  if (!loginForm) return;

  loginForm.addEventListener("submit", async (e) => {
    e.preventDefault();

    const username = loginForm.querySelector("#username").value.trim();
    const password = loginForm.querySelector("#password").value;

    await handleLogin(username, password);
  });
}

/**
 * Gestiona la respuesta del intento de login.
 * @param {string} username
 * @param {string} password
 */
async function handleLogin(username, password) {
  const loginBtn = document.querySelector(".login-btn");
  const originalText = loginBtn.innerHTML;

  loginBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Validando...';
  loginBtn.disabled = true;

  try {
    const res = await window.iniciar_sesion(username, password);
    console.log("iniciar_sesion():", res);

    const success =
      res.status === "ok" && res.type === "boolean" && res.data === true;

    if (success) {
      showSuccess(res.message || "¡Bienvenido de vuelta!");
      setTimeout(() => {
        window.location.reload();
      }, 500);
    } else {
      showError(res.message || "Usuario o contraseña incorrectos.");
    }
  } catch (err) {
    showError("Error de conexión. Intenta más tarde.");
    console.error(err);
  } finally {
    loginBtn.innerHTML = originalText;
    loginBtn.disabled = false;
  }
}

/**
 * Añade efectos de hover al botón de registro.
 */
function setupRegisterButtonHover() {
  const registerBtn = document.querySelector(".register-btn");
  if (!registerBtn) return;

  registerBtn.addEventListener("mouseenter", () => {
    registerBtn.style.transform = "translateY(-3px)";
  });

  registerBtn.addEventListener("mouseleave", () => {
    registerBtn.style.transform = "translateY(0)";
  });
}

/**
 * Muestra un mensaje de error debajo del formulario.
 * @param {string} message
 */
function showError(message) {
  clearMessages(".error-message");
  createMessage(message, "error-message", "#ff6b6b");
}

/**
 * Muestra un mensaje de éxito debajo del formulario.
 * @param {string} message
 */
function showSuccess(message) {
  clearMessages(".success-message");
  createMessage(message, "success-message", "#4cc9f0");
}

/**
 * Crea y muestra un mensaje temporal debajo del formulario.
 * @param {string} text
 * @param {string} className
 * @param {string} color
 */
function createMessage(text, className, color) {
  const form = document.querySelector(".login-form");
  if (!form) return;

  const messageEl = document.createElement("div");
  messageEl.className = className;
  messageEl.textContent = text;
  messageEl.style.color = color;
  messageEl.style.marginTop = "1rem";

  form.appendChild(messageEl);

  setTimeout(() => {
    messageEl.style.opacity = "0";
    setTimeout(() => messageEl.remove(), 300);
  }, 3000);
}

/**
 * Elimina mensajes existentes de un tipo.
 * @param {string} selector
 */
function clearMessages(selector) {
  const existingMessages = document.querySelectorAll(selector);
  existingMessages.forEach((msg) => msg.remove());
}
