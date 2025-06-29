#include <utils/async.h>
#include <windows.h>
#include <stdlib.h>

// Para encapsular un callback
typedef struct
{
    // Argumento para el callback
    void *arg;

    // Callback
    callback_t callback;
} callback_wrapper_arg_t;

// Para encapsular el callback en el formato que requiere CreateThread()
DWORD __stdcall callback_wrapper(void *wrapper_arg)
{
    callback_wrapper_arg_t *arg = wrapper_arg;
    arg->callback(arg->arg);
    free(wrapper_arg);
    return 0;
}

bool new_thread(callback_t callback, void *arg)
{
    callback_wrapper_arg_t *wrapper_arg =
        malloc(sizeof(callback_wrapper_arg_t));

    wrapper_arg->arg = arg;
    wrapper_arg->callback = callback;

    HANDLE thread =
        CreateThread(
            NULL, 0,
            callback_wrapper,
            wrapper_arg,
            0, NULL);

    if (!thread)
    {
        free(wrapper_arg);
        return false;
    }

    CloseHandle(thread);
    return true;
}