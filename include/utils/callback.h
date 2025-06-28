#ifndef VIBECAST_CALLBACK_H
#define VIBECAST_CALLBACK_H 1

// Tipo para un callback
typedef int (*callback_t)(void *arg);

// Macro para definir callbacks
#define new_callback(callback_name) int callback_name(void *arg)

#endif // VIBECAST_CALLBACK_H