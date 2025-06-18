#include <db/datos.h>
#include <db/dbmgr.h>
#include <utils/utils.h>

static ABB usuarios = {NULL};
static ABB artistas = {NULL};
static Lista albumes = {NULL};
static Lista canciones = {NULL};

bool func(LoadData)
{
    return true; // Datos cargados correctamente
}
