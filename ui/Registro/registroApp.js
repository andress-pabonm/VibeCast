document.addEventListener('DOMContentLoaded', function() {
    // ========== SELECTORES ==========
    const form = document.querySelector('.register-form');
    const inputs = document.querySelectorAll('.form-input');
    const passwordInput = document.querySelector('input[type="password"]');
    const togglePassword = document.querySelector('.toggle-password');
    const registerBtn = document.querySelector('.register-btn');
    const termsCheckbox = document.querySelector('.terms-checkbox');
    const termsLabel = document.querySelector('.terms-label');
    const registerContainer = document.querySelector('.register-container');
    
    // ========== VALIDACIÓN DE FORMULARIO ==========
    inputs.forEach(input => {
        // Validación al perder el foco
        input.addEventListener('blur', validateInput);
        
        // Reset al empezar a escribir
        input.addEventListener('input', () => {
            const parent = input.closest('.input-group');
            parent.classList.remove('error', 'success');
        });
    });
    
    function validateInput(e) {
        const input = e.target;
        const parent = input.closest('.input-group');
        const errorMessage = parent.querySelector('.error-message');
        
        if (!input.value.trim()) {
            parent.classList.add('error');
            errorMessage.textContent = 'Este campo es requerido';
            errorMessage.style.display = 'block';
        } else {
            // Validación específica para email
            if (input.type === 'email' && !isValidEmail(input.value)) {
                parent.classList.add('error');
                errorMessage.textContent = 'Ingresa un email válido';
                errorMessage.style.display = 'block';
                return;
            }
            
            // Validación específica para contraseña
            if (input.type === 'password' && input.value.length < 6) {
                parent.classList.add('error');
                errorMessage.textContent = 'La contraseña debe tener al menos 6 caracteres';
                errorMessage.style.display = 'block';
                return;
            }
            
            parent.classList.remove('error');
            parent.classList.add('success');
            errorMessage.style.display = 'none';
        }
    }
    
    function isValidEmail(email) {
        return /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email);
    }
    
    // ========== TOGGLE PASSWORD ==========
    if (togglePassword && passwordInput) {
        togglePassword.addEventListener('click', function() {
            const type = passwordInput.getAttribute('type') === 'password' ? 'text' : 'password';
            passwordInput.setAttribute('type', type);
            
            // Cambiar icono
            this.classList.toggle('fa-eye');
            this.classList.toggle('fa-eye-slash');
            
            // Efecto visual
            this.style.transform = 'scale(1.1)';
            setTimeout(() => {
                this.style.transform = 'scale(1)';
            }, 200);
        });
    }
    
    // ========== EFECTOS DE BOTÓN ==========
    if (registerBtn) {
        // Efecto hover
        registerBtn.addEventListener('mouseenter', () => {
            registerBtn.style.transform = 'translateY(-3px)';
            registerBtn.style.boxShadow = '0 8px 25px var(--spider-azul)';
        });
        
        registerBtn.addEventListener('mouseleave', () => {
            registerBtn.style.transform = 'translateY(0)';
            registerBtn.style.boxShadow = '0 5px 15px rgba(67, 97, 238, 0.4)';
        });
        
        // Efecto click
        registerBtn.addEventListener('mousedown', () => {
            registerBtn.style.transform = 'translateY(1px)';
        });
        
        registerBtn.addEventListener('mouseup', () => {
            registerBtn.style.transform = 'translateY(-3px)';
        });
    }
    
    // ========== ANIMACIÓN DE CHECKBOX ==========
    if (termsCheckbox && termsLabel) {
        termsLabel.addEventListener('click', () => {
            const customCheckbox = termsLabel.querySelector('.custom-checkbox');
            
            if (termsCheckbox.checked) {
                customCheckbox.style.transform = 'scale(0.9)';
                setTimeout(() => {
                    customCheckbox.style.transform = 'scale(1)';
                }, 150);
            }
        });
    }
    
    // ========== ENVÍO DE FORMULARIO ==========
    if (form) {
        form.addEventListener('submit', function(e) {
            e.preventDefault();
            
            // Validar todos los campos antes de enviar
            let isValid = true;
            inputs.forEach(input => {
                const event = { target: input };
                validateInput(event);
                
                const parent = input.closest('.input-group');
                if (parent.classList.contains('error')) {
                    isValid = false;
                }
            });
            
            // Validar términos y condiciones
            if (!termsCheckbox.checked) {
                termsLabel.style.color = 'var(--spider-error)';
                isValid = false;
                
                // Animación de shake
                termsLabel.style.animation = 'shake 0.5s';
                setTimeout(() => {
                    termsLabel.style.animation = '';
                }, 500);
            } else {
                termsLabel.style.color = 'var(--spider-detalle)';
            }
            
            if (!isValid) {
                // Efecto de error en el botón
                registerBtn.style.background = 'var(--spider-error)';
                setTimeout(() => {
                    registerBtn.style.background = 'linear-gradient(45deg, var(--spider-morado), var(--spider-azul-oscuro))';
                }, 1000);
                return;
            }
            
            // Animación de carga
            registerBtn.innerHTML = '<i class="fas fa-spinner fa-spin btn-icon"></i> Creando tu cuenta...';
            registerBtn.disabled = true;
            
            // Simular envío al servidor
            setTimeout(() => {
                registerBtn.innerHTML = '<i class="fas fa-check btn-icon"></i> ¡Registro exitoso!';
                registerBtn.style.background = 'var(--spider-azul)';
                
                // Efecto de confeti (simplificado)
                createConfetti();
                
                // Redirigir después de 2 segundos
                setTimeout(() => {
                    window.location.href = 'welcome.html'; // Cambia por tu URL
                }, 2000);
            }, 2000);
        });
    }
    
    // ========== EFECTO CONFETI ==========
    function createConfetti() {
        const colors = ['var(--spider-morado)', 'var(--spider-azul)', 'var(--spider-azul-oscuro)', 'var(--spider-blanco)'];
        
        for (let i = 0; i < 100; i++) {
            const confetti = document.createElement('div');
            confetti.style.position = 'fixed';
            confetti.style.width = '8px';
            confetti.style.height = '8px';
            confetti.style.backgroundColor = colors[Math.floor(Math.random() * colors.length)];
            confetti.style.borderRadius = '50%';
            confetti.style.left = Math.random() * 100 + 'vw';
            confetti.style.top = '-10px';
            confetti.style.zIndex = '1000';
            confetti.style.opacity = '0.8';
            
            // Animación
            const animationDuration = Math.random() * 3 + 2;
            confetti.style.animation = `fall ${animationDuration}s linear forwards`;
            
            document.body.appendChild(confetti);
            
            // Eliminar después de la animación
            setTimeout(() => {
                confetti.remove();
            }, animationDuration * 1000);
        }
        
        // Añadir keyframes dinámicamente
        const style = document.createElement('style');
        style.innerHTML = `
            @keyframes fall {
                to {
                    transform: translateY(100vh) rotate(360deg);
                    opacity: 0;
                }
            }
            
            @keyframes shake {
                0%, 100% { transform: translateX(0); }
                10%, 30%, 50%, 70%, 90% { transform: translateX(-5px); }
                20%, 40%, 60%, 80% { transform: translateX(5px); }
            }
        `;
        document.head.appendChild(style);
    }
    
    // ========== EFECTO SCROLL ==========
    if (registerContainer) {
        // Inicialmente transparente
        registerContainer.style.opacity = '0';
        registerContainer.style.transform = 'translateY(20px)';
        registerContainer.style.transition = 'opacity 0.5s ease, transform 0.5s ease';
        
        // Mostrar con efecto al cargar
        setTimeout(() => {
            registerContainer.style.opacity = '1';
            registerContainer.style.transform = 'translateY(0)';
        }, 300);
        
        // Efecto al hacer scroll
        window.addEventListener('scroll', () => {
            const containerPosition = registerContainer.getBoundingClientRect().top;
            const screenPosition = window.innerHeight / 1.3;
            
            if (containerPosition < screenPosition) {
                registerContainer.style.opacity = '1';
                registerContainer.style.transform = 'translateY(0)';
            }
        });
    }
    
    // ========== CAMBIO DE TEMA (OPCIONAL) ==========
    const themeToggle = document.createElement('div');
    themeToggle.innerHTML = '<i class="fas fa-palette"></i>';
    themeToggle.style.position = 'fixed';
    themeToggle.style.bottom = '20px';
    themeToggle.style.right = '20px';
    themeToggle.style.background = 'var(--spider-morado)';
    themeToggle.style.color = 'white';
    themeToggle.style.width = '40px';
    themeToggle.style.height = '40px';
    themeToggle.style.borderRadius = '50%';
    themeToggle.style.display = 'flex';
    themeToggle.style.justifyContent = 'center';
    themeToggle.style.alignItems = 'center';
    themeToggle.style.cursor = 'pointer';
    themeToggle.style.zIndex = '1000';
    themeToggle.style.boxShadow = '0 2px 10px rgba(0,0,0,0.2)';
    document.body.appendChild(themeToggle);
    
    let isDefaultTheme = true;
    themeToggle.addEventListener('click', () => {
        if (isDefaultTheme) {
            // Cambiar a tema oscuro alternativo
            document.documentElement.style.setProperty('--spider-morado', '#121212');
            document.documentElement.style.setProperty('--spider-morado-oscuro', '#000000');
            document.documentElement.style.setProperty('--spider-azul', '#ffffff');
            document.documentElement.style.setProperty('--spider-blanco', '#e0e0e0');
        } else {
            // Volver al tema original
            document.documentElement.style.setProperty('--spider-morado', '#8d2de2');
            document.documentElement.style.setProperty('--spider-morado-oscuro', '#5e1dc7');
            document.documentElement.style.setProperty('--spider-azul', '#4cc9f0');
            document.documentElement.style.setProperty('--spider-blanco', '#f1faee');
        }
        isDefaultTheme = !isDefaultTheme;
    });
});