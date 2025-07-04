#include <ui/interfaces.h>
#include <utils/utils.h>

void activarPremium(){
    usuario->plan = PLAN_PREMIUM; // Cambiamos el plan del usuario a premium
    printf("¡Felicidades! Has activado el plan Premium.\n");

}
void desactivarPremium(){

    usuario->plan = PLAN_FREEMIUM; // Cambiamos el plan del usuario a freemium
    printf("Has desactivado el plan Premium. Ahora estás en el plan Freemium.\n");
    
}
bool isPremium(){
    return usuario->plan == PLAN_PREMIUM; // Retorna true si el usuario es premium
}
void renovarpremium(){
    activarPremium(); // Llama a la función para activar el plan premium
    printf("Tu plan Premium ha sido renovado exitosamente.\n");
}