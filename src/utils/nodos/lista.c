#include <utils/nodos/lista.h>
#include <utils/nodos/constants.h>

#include <utils/memmgr.h>
#include <utils/macros.h>

#include <stddef.h>

// ========================================
// Estructuras internas
// ========================================

typedef struct Nodo
{
    struct Nodo *r;
    void *v;
} *Nodo;

typedef struct __Lista
{
    Nodo start;
    cmpfn_t cmp; // Función de comparación asociada a esta lista
} *__Lista;

typedef struct
{
    operfn_t callback;
    void *arg;
} foreach_wrapper_arg_t;

typedef struct
{
    cmpfn_t cmp;
    Nodo nodo;
    int *index;
} insert_wrapper_arg_t;

typedef struct
{
    cmpfn_t cmp;
    void *value_ptr;
    void *found;
} search_wrapper_arg_t;

// ========================================
// Funciones auxiliares
// ========================================

static Nodo newNodo(void *value_ptr)
{
    return value_ptr
               ? alloc(
                     struct Nodo,
                     &cast(
                         struct Nodo,
                         .r = NULL,
                         .v = value_ptr))
               : NULL;
}

static void destroyNodo(Nodo nodo)
{
    freem(nodo);
}

// ========================================
// Recorridos internos (Nodo*)
// ========================================

static void forEachInLista_Ref(Lista lista, operfn_t callback, void *arg)
{
    if (!lista || !callback)
        return;

    Nodo *ref = &cast(__Lista, lista)->start;
    int i = 0;

    while (ref)
    {
        switch (callback(arg, i, ref))
        {
        case FOREACH_CONTINUE:
            ref = *ref ? &(*ref)->r : NULL;
            i++;
            break;
        case FOREACH_BREAK:
            return;
        case FOREACH_STAY:
            break;
        default:
            return;
        }
    }
}

// ========================================
// Wrappers intermedios
// ========================================

static new_operfn(foreach_wrapper_Lista_Nodo)
{
    if (!value_ptr || !arg)
        return FOREACH_BREAK;

    foreach_wrapper_arg_t *wrapper_arg = arg;

    if (!wrapper_arg->callback)
        return FOREACH_BREAK;

    return wrapper_arg->callback(
        wrapper_arg->arg,
        index,
        *cast(Nodo *, value_ptr));
}

static new_operfn(foreach_wrapper_Lista_Value)
{
    if (!value_ptr || !arg)
        return FOREACH_BREAK;

    foreach_wrapper_arg_t *wrapper_arg = arg;

    if (!wrapper_arg->callback)
        return FOREACH_BREAK;

    return wrapper_arg->callback(
        wrapper_arg->arg,
        index,
        cast(Nodo, value_ptr)->v);
}

// ========================================
// API Pública - Recorrido
// ========================================

static void forEachInLista_Nodo(Lista lista, operfn_t callback, void *arg)
{
    foreach_wrapper_arg_t wrapper_arg = {
        .callback = callback,
        .arg = arg};
    forEachInLista_Ref(lista, foreach_wrapper_Lista_Nodo, &wrapper_arg);
}

void forEachInLista(Lista lista, operfn_t callback, void *arg)
{
    foreach_wrapper_arg_t wrapper_arg = {
        .callback = callback,
        .arg = arg};
    forEachInLista_Nodo(lista, foreach_wrapper_Lista_Value, &wrapper_arg);
}

// ========================================
// API Pública - Creación
// ========================================

Lista newLista(cmpfn_t cmp)
{
    return alloc(struct __Lista,
                 &cast(struct __Lista,
                       .start = NULL,
                       .cmp = cmp));
}

// ========================================
// API Pública - Inserción
// ========================================

static new_operfn(insert_wrapper_Lista)
{
    insert_wrapper_arg_t *wrapper_arg = arg;
    Nodo *ref = value_ptr;

    if (*ref && (!wrapper_arg->cmp ||
                 wrapper_arg->cmp((*ref)->v,
                                  wrapper_arg->nodo->v) < 0))
        return FOREACH_CONTINUE;

    wrapper_arg->nodo->r = *ref;
    *ref = wrapper_arg->nodo;
    *(wrapper_arg->index) = index;

    return FOREACH_BREAK;
}

int insertValueInLista(Lista lista, void *value_ptr)
{
    if (!lista)
        return INSERT_FAILED;

    Nodo nodo = newNodo(value_ptr);
    if (!nodo)
        return INSERT_FAILED;

    int i = INSERT_FAILED;
    insert_wrapper_arg_t wrapper_arg = {
        .cmp = cast(__Lista, lista)->cmp,
        .nodo = nodo,
        .index = &i};

    forEachInLista_Ref(lista, insert_wrapper_Lista, &wrapper_arg);
    return i;
}

// ========================================
// API Pública - Búsqueda
// ========================================

static new_operfn(search_wrapper_Lista)
{
    search_wrapper_arg_t *wrapper_arg = arg;

    if (wrapper_arg->cmp(value_ptr, wrapper_arg->value_ptr))
        return FOREACH_CONTINUE;

    wrapper_arg->found = value_ptr;
    return FOREACH_BREAK;
}

static new_operfn(search_wrapper_Lista_Ref)
{
    search_wrapper_arg_t *wrapper_arg = arg;

    if (wrapper_arg->cmp((*cast(Nodo *, value_ptr))->v, wrapper_arg->value_ptr))
        return FOREACH_CONTINUE;

    wrapper_arg->found = value_ptr;
    return FOREACH_BREAK;
}

void *searchValueInLista(Lista lista, const void *value_ptr, cmpfn_t cmp)
{
    if (!lista || !value_ptr || !cmp)
        return NULL;

    search_wrapper_arg_t wrapper_arg = {
        .cmp = cmp,
        .value_ptr = value_ptr,
        .found = NULL};

    forEachInLista(lista, search_wrapper_Lista, &wrapper_arg);
    return wrapper_arg.found;
}

static Nodo *searchValueInLista_Ref(Lista lista, void *value_ptr, cmpfn_t cmp)
{
    if (!lista || !value_ptr || !cmp)
        return NULL;

    search_wrapper_arg_t wrapper_arg = {
        .cmp = cmp,
        .value_ptr = value_ptr,
        .found = NULL};

    forEachInLista_Ref(lista, search_wrapper_Lista_Ref, &wrapper_arg);
    return wrapper_arg.found;
}

// ========================================
// API Pública - Eliminación
// ========================================

void *deleteValueInLista(Lista lista, const void *value_ptr, cmpfn_t cmp)
{
    Nodo *ref = searchValueInLista_Ref(lista, value_ptr, cmp);
    if (!ref)
        return NULL;

    Nodo nodo = *ref;
    *ref = nodo->r;

    value_ptr = nodo->v;
    destroyNodo(nodo);
    return value_ptr;
}

// ========================================
// API Pública - Destrucción
// ========================================

static new_operfn(destroy_wrapper_Ref)
{
    if (!value_ptr)
        return FOREACH_BREAK;

    Nodo *ref = value_ptr;
    Nodo nodo = *ref;

    if (!nodo)
        return FOREACH_BREAK;

    foreach_wrapper_Lista_Value(arg, index, nodo);

    *ref = nodo->r;
    destroyNodo(nodo);

    return FOREACH_STAY;
}

void destroyLista(Lista lista, operfn_t callback, void *arg)
{
    foreach_wrapper_arg_t wrapper_arg = {
        .callback = callback,
        .arg = arg};

    forEachInLista_Ref(lista, destroy_wrapper_Ref, &wrapper_arg);
    freem(lista);
}
