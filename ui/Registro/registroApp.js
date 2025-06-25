/*
 * Manejo del registro
 * Efectos interactivos
 * Validación básica
 */

document.addEventListener("DOMContentLoaded", () => {
  window.is_logged_in().then((res) => {
    if (res.message === 1) {
      location.hrem = "../Menu/menu.html";
    }
  });

  registerForm.addEventListener("submit", (e) => {
    e.preventDefault();

    const nickname = registerForm.querySelector("#name");
    const pais = registerForm.querySelector("#country");
    const username = registerForm.querySelector("#username");
    const email = registerForm.querySelector("#email");
    const password = registerForm.querySelector("#password");
    const confirmPassword = registerForm.querySelector("#confirmPassword");

    simulateRegister(
      nickname.value,
      pais.value,
      username.value,
      email.value,
      password.value,
      confirmPassword.value
    );
  });

  // ========== SELECTORES ==========

  // ========== MOSTRAR/OCULTAR CONTRASEÑA ==========
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

  // ========== EFECTOS DE BOTÓN ==========
  const registerBtn = document.querySelector(".register-btn");
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
 * Simula el proceso de registro
 */
function simulateRegister(
  nickname,
  pais,
  username,
  email,
  password,
  confirmPassword
) {
  // Simulación de carga
  const registerBtn = document.querySelector(".register-btn");
  const originalText = registerBtn.innerHTML;

  registerBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Validando...';
  registerBtn.disabled = true;

  window
    .crear_cuenta(nickname, pais, username, email, password, confirmPassword)
    .then((res) => {
      registerBtn.innerHTML = originalText;
      registerBtn.disabled = false;
      console.log("crear_cuenta(): ", res);
      if (res.message === "1") {
        // showSuccess("¡Registro completo!");
        setTimeout(() => {
          window.location.replace("../Login/index.html");
        }, 500);
      } else {
        console.log(res.message);
        // showError("Usuario o contraseña incorrectos.");
      }
    });
}
