#ifndef VIBECAST_ASYNC_H
#define VIBECAST_ASYNC_H 1

#include <stdbool.h>
#include "callback.h"

// Crea un nuevo hilo para ejecutar el callback dado con el argumento dado
bool new_thread(callback_t callback, void *arg);

// Crea un hilo que ejecuta un callback después de (ms) milisegundos
void setTimeout(callback_t callback, void *arg, int ms);

#endif // VIBECAST_ASYNC_H