/* #ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

// Only used to suppress warnings caused by unused parameters.
#define UNUSED(x) (void)x

typedef struct {
  void *arg;
  void (*next_fn)(void *);
  void (*free_fn)(void *);
} start_routine_wrapper_arg_t;

#ifdef _WIN32
DWORD __stdcall start_routine_wrapper(void *wrapper_arg) {
  start_routine_wrapper_arg_t *arg = (start_routine_wrapper_arg_t *)wrapper_arg;
  arg->next_fn(arg->arg);
  arg->free_fn(wrapper_arg);
  return 0;
}
#else
void *start_routine_wrapper(void *wrapper_arg) {
  start_routine_wrapper_arg_t *arg = (start_routine_wrapper_arg_t *)wrapper_arg;
  arg->next_fn(arg->arg);
  arg->free_fn(wrapper_arg);
  return NULL;
}
#endif

// Creates a thread with the given start routine and argument passed to
// the start routine. Returns 0 on success and -1 on failure.
int thread_create(void (*start_routine)(void *), void *arg) {
  start_routine_wrapper_arg_t *wrapper_arg =
      (start_routine_wrapper_arg_t *)malloc(
          sizeof(start_routine_wrapper_arg_t));
  wrapper_arg->arg = arg;
  wrapper_arg->next_fn = start_routine;
  wrapper_arg->free_fn = free;

#ifdef _WIN32
  HANDLE thread =
      CreateThread(NULL, 0, start_routine_wrapper, wrapper_arg, 0, NULL);
  if (thread) {
    CloseHandle(thread);
    return 0;
  }
  return -1;
#else
  pthread_t thread;
  int error = pthread_create(&thread, NULL, start_routine_wrapper, wrapper_arg);
  if (error == 0) {
    pthread_detach(thread);
    return 0;
  }
  return -1;
#endif
}

// Make the current thread sleep for the given number of seconds.
void thread_sleep(int seconds) {
#ifdef _WIN32
  Sleep(seconds * 1000);
#else
  sleep(seconds);
#endif
}

typedef struct {
  webview_t w;
  long count;
} context_t;

static const char html[] = "\
<div>\n\
  <button id=\"increment\">+</button>\n\
  <button id=\"decrement\">−</button>\n\
  <span>Counter: <span id=\"counterResult\">0</span></span>\n\
</div>\n\
<hr />\n\
<div>\n\
  <button id=\"compute\">Compute</button>\n\
  <span>Result: <span id=\"computeResult\">(not started)</span></span>\n\
</div>\n\
<script type=\"module\">\n\
  const getElements = ids => Object.assign({}, ...ids.map(\n\
    id => ({ [id]: document.getElementById(id) })));\n\
  const ui = getElements([\n\
    \"increment\", \"decrement\", \"counterResult\", \"compute\",\n\
    \"computeResult\"\n\
  ]);\n\
  ui.increment.addEventListener(\"click\", async () => {\n\
    ui.counterResult.textContent = await window.count(1);\n\
  });\n\
  ui.decrement.addEventListener(\"click\", async () => {\n\
    ui.counterResult.textContent = await window.count(-1);\n\
  });\n\
  ui.compute.addEventListener(\"click\", async () => {\n\
    ui.compute.disabled = true;\n\
    ui.computeResult.textContent = \"(pending)\";\n\
    ui.computeResult.textContent = await window.compute(6, 7);\n\
    ui.compute.disabled = false;\n\
  });\n\
</script>";

void count(const char *id, const char *req, void *arg) {
  context_t *context = (context_t *)arg;
  // Imagine that params->req is properly parsed or use your own JSON parser.
  long direction = strtol(req + 1, NULL, 10);
  char result[10] = {0};
  (void)sprintf(result, "%ld", context->count += direction);
  webview_return(context->w, id, 0, result);
}

typedef struct {
  webview_t w;
  char *id;
  char *req;
} compute_thread_params_t;

compute_thread_params_t *
compute_thread_params_create(webview_t w, const char *id, const char *req) {
  compute_thread_params_t *params =
      (compute_thread_params_t *)malloc(sizeof(compute_thread_params_t));
  params->w = w;
  params->id = (char *)malloc(strlen(id) + 1);
  params->req = (char *)malloc(strlen(req) + 1);
  strcpy(params->id, id);
  strcpy(params->req, req);
  return params;
}

void compute_thread_params_free(compute_thread_params_t *p) {
  free(p->req);
  free(p->id);
  free(p);
}

void compute_thread_proc(void *arg) {
  compute_thread_params_t *params = (compute_thread_params_t *)arg;
  // Simulate load.
  thread_sleep(1);
  // Imagine that params->req is properly parsed or use your own JSON parser.
  const char *result = "42";
  webview_return(params->w, params->id, 0, result);
  compute_thread_params_free(params);
}

void compute(const char *id, const char *req, void *arg) {
  context_t *context = (context_t *)arg;
  compute_thread_params_t *params =
      compute_thread_params_create(context->w, id, req);
  // Create a thread and forget about it for the sake of simplicity.
  if (thread_create(compute_thread_proc, params) != 0) {
    compute_thread_params_free(params);
  }
} */