#include <ui/interfaces.h>
#include <utils/utils.h>

#include <pcre2posix.h> // Para expresiones regulares
#include <json.h>

static Usuario *usuario = NULL;

bool iniciar_sesion(const char *data)
{
    if (usuario)     // Si ya ha iniciado sesión
        return true; // Directamente retornar true

    // TODO 1: Obtener los datos desde (data) (JSON)
    // TODO 2: Validar email
    // TODO 3: Buscar email en los datos
    // TODO 4: Verificar que la contraseña sea correcta
    // TODO 5: Si los datos son correctos, establecer el puntero (usuario)
    // TODO 6: Continuar con la interfaz principal de la aplicación

    return true; // Sesión iniciada correctamente
}

bool crear_cuenta(const char *data)
{
    // TODO 1: Obtener los datos desde (data) (JSON)
    // TODO 2: Validar que todos los campos estén definidos
    // TODO 3: Validar el email
    // TODO 4: Buscar por si el email ya está registrado
    // TODO 5: Si el email no está registrado, crear la estructura del usuario
    // TODO 6: Una vez creada la estructura, agregarla a la base de datos como un nuevo registro
    // TODO 7: Si no hubo problema para registrarlo en la base de datos, insertar el usuario en el ABB
    // TODO 8: Regresar a la interfaz de inicio de sesión

    return true; // Registro completo
}

bool cerrar_sesion()
{
    if (!usuario)    // Si no hay ninguna sesión abierta
        return true; // Entonces no hay ninguna sesión que cerrar

    // TODO 1: Establecer (usuario) como NULL
    // TODO 2: Volver a la interfaz de inicio de sesión

    return true; // Sesión cerrada correctamente
}
