// Para asignar memoria en HEAP
#include <stdlib.h>

/* Incluye las librerías para usar hilos */
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

// Estructura para representar una rutina que se ejecutará en un hilo
typedef struct
{
    // Argumento para la rutina
    void *arg;

    // Rutina
    void (*next_fn)(void *);

    // Función para liberar la memoria de esta estructura
    void (*free_fn)(void *);
} start_routine_wrapper_arg_t;

// Función para ejecutar una rutina
#ifdef _WIN32
static DWORD __stdcall start_routine_wrapper(void *wrapper_arg)
#else
static void *start_routine_wrapper(void *wrapper_arg)
#endif
{
    start_routine_wrapper_arg_t *arg = (start_routine_wrapper_arg_t *)wrapper_arg;
    arg->next_fn(arg->arg);
    arg->free_fn(wrapper_arg);
    return 0;
}

// Crea un nuevo hilo para ejecutar una rutina
// Retorna 0 si pudo crear el hilo, -1 si no puedo crear el hilo
int thread_create(void (*start_routine)(void *), void *arg)
{
    // Crea la estructura para encapsular la rutina
    start_routine_wrapper_arg_t *wrapper_arg =
        (start_routine_wrapper_arg_t *)malloc(
            sizeof(start_routine_wrapper_arg_t));
    wrapper_arg->arg = arg;
    wrapper_arg->next_fn = start_routine;
    wrapper_arg->free_fn = free;

    // Crea el hilo
#ifdef _WIN32
    HANDLE thread =
        CreateThread(NULL, 0,
                     start_routine_wrapper, // Ejecuta esta función
                     wrapper_arg,           // Con este argumento
                     0, NULL);
    if (thread)
    {
        CloseHandle(thread);
        return 0;
    }
    return -1;
#else
    pthread_t thread;
    int error =
        pthread_create(&thread, NULL,
                       start_routine_wrapper, // Ejecuta esta función
                       wrapper_arg);          // Con este argumento
    if (error == 0)
    {
        pthread_detach(thread);
        return 0;
    }
    return -1;
#endif
}