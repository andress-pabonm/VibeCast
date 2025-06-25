#ifndef ASYNC_H
#define ASYNC_H 1

int thread_create(void (*start_routine)(void *), void *arg);

#endif // ASYNC_H