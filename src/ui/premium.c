#include <ui/interfaces.h>
#include <utils/utils.h>
#include <time.h>

typedef struct usuarioAUX{
    void *Usuario; 
    time_t caducidadPremium; // Fecha de caducidad del plan premium
}usuarioAUX;

void activarPremium(usuarioAUX *usuario) {

    time_t base = time(NULL);

    if (base == -1) {
        printf("Error al obtener la fecha actual.\n");
        return;
    }
    
    Usuario *user = (Usuario *)usuario->Usuario;
    if (user->plan != PLAN_PREMIUM) {
        user->plan = PLAN_PREMIUM;
        printf("¡Plan Premium activado! ");
    } else {
        base = usuario->caducidadPremium;
        printf("¡Plan Premium renovado! ");
    }

    usuario->caducidadPremium = base + 30 * 24 * 60 * 60; //Tiempo en segundos
    //La fecha es imprime como dia de la semana, mes, dia del mes, hora, minuto, segundo y año
    printf("Caduca el: %s", ctime(&usuario->caducidadPremium));
}

void desactivarPremium(usuarioAUX *usuario) {

    Usuario *user = (Usuario *)usuario->Usuario;
    user->plan = PLAN_FREEMIUM; // Cambiamos el plan del usuario a freemium
    usuario->caducidadPremium = 0; // Reseteamos la fecha de caducidad del plan premium
    printf("Has desactivado el plan Premium. Ahora estás en el plan Freemium.\n");
    
}
bool isPremium(usuarioAUX *usuario) {

    time_t ahora = time(NULL);
    if (ahora == -1) {
        printf("Error al obtener la fecha actual.\n");
        return false;
    }

    if(usuario->caducidadPremium <= ahora){
        printf("Tu plan Premium ha caducado. Por favor, renueva tu suscripción.\n");
        desactivarPremium(usuario); // Desactiva el plan si ha caducado
        return false; // Retorna false si el usuario no es premium
    }

    Usuario *user = (Usuario *)usuario->Usuario;
    return user->plan == PLAN_PREMIUM; // Retorna true si el usuario es premium
}
void renovarpremium(usuarioAUX *usuario) {
    activarPremium(usuario); // Llama a la función para activar el plan premium
    printf("Tu plan Premium ha sido renovado exitosamente.\n");
}