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

#define sleep(ms) Sleep(ms);

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

typedef struct
{
    callback_t callback;
    void *arg;
    int ms;
} timeout_wrapper_arg_t;

static new_callback(timeout_wrapper)
{
    timeout_wrapper_arg_t *wrapper_arg = arg;

    sleep(wrapper_arg->ms);
    int r = wrapper_arg->callback(wrapper_arg->arg);
    free(wrapper_arg);

    return r;
}

void setTimeout(callback_t callback, void *arg, int ms)
{
    timeout_wrapper_arg_t *wrapper_arg =
        malloc(sizeof(timeout_wrapper_arg_t));

    wrapper_arg->callback = callback;
    wrapper_arg->arg = arg;
    wrapper_arg->ms = ms;

    if (!new_thread(timeout_wrapper, wrapper_arg))
        free(wrapper_arg);
}