#include <ui/login.h>
#include <utils/utils.h>

static const char *opcv[] = {
    "Opción 1",
    "Opción 2",
    "Opción 3"};
static const int opcc = sizeof(opcv) / sizeof(*opcv);

SDL_AppResult VibeCast_LoginUI(VibeCast_InterfazID *id)
{
    switch (menu("Iniciar sesión", opcc, opcv))
    {
    case 1: // Opción 1
        puts("Opción 1");
        break;

    case 2: // Opción 2
        puts("Opción 2");
        break;

    case 3: // Opción 3
        puts("Opción 3");
        return SDL_APP_SUCCESS;

    default:
        puts("Opción no válida.");
    }

    puts("Pulsa ENTER para continuar.");
    waitAndClear();
    
    return SDL_APP_CONTINUE;
}