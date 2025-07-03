document.addEventListener("DOMContentLoaded", () => {
    checkUserLoggedIn();
    setupRegisterForm();
    setupPasswordToggle();
    setupRegisterButtonHover();
});

// Verifica si ya hay sesión activa.
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

// Configura el envío del formulario de registro.
function setupRegisterForm() {
    const registerForm = document.querySelector(".register-form");
    if (!registerForm) return;

    registerForm.addEventListener("submit", async (e) => {
        e.preventDefault();

        const nickname = registerForm.querySelector("#name").value.trim();
        const pais = registerForm.querySelector("#country").value.trim();
        const username = registerForm.querySelector("#username").value.trim();
        const email = registerForm.querySelector("#email").value.trim();
        const password = registerForm.querySelector("#password").value;
        const confirmPassword = registerForm.querySelector("#confirmPassword").value;

        await handleRegister(
            nickname,
            pais,
            username,
            email,
            password,
            confirmPassword
        );
    });
}

// Maneja el proceso de registro de usuario.
async function handleRegister(nickname, pais, username, email, password, confirmPassword) {
    const registerBtn = document.querySelector(".register-btn");
    const originalText = registerBtn.innerHTML;

    registerBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Validando...';
    registerBtn.disabled = true;

    try {  
        const res = await window.crear_cuenta(nickname, pais, username, email, password, confirmPassword);
        console.log("crear_cuenta():", res);

        const success =
            res.status === "ok" && res.type === "boolean" && res.data === true;

        if (success) {
            showSuccess(res.message || "¡Cuenta creada exitosamente!");
            setTimeout(() => {
                window.location.replace("../Login/index.html");
            }, 500);
        } else {
            showError(res.message || "No se pudo crear la cuenta.");
        }
    } catch (err) {
        showError("Error de conexión. Intenta más tarde.");
        console.error(err);
    } finally {
        registerBtn.innerHTML = originalText;
        registerBtn.disabled = false;
    }
}

// Activa el efecto de mostrar/ocultar contraseña.
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

// Activa efecto de hover en el botón de registro.
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
    const form = document.querySelector(".register-form");
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
