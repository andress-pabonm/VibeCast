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
    struct Nodo *der;
    void *val;
} *Nodo;

typedef struct __Lista
{
    Nodo start;
    cmpfn_t cmp; // Función de comparación asociada a esta lista
} *__Lista;

typedef struct
{
    operfn_t cb;
    void *arg;
} foreach_wrapper_arg_t;

typedef struct
{
    cmpfn_t cmp;
    Nodo nodo;
    int *idx;
} insert_wrapper_arg_t;

typedef struct
{
    cmpfn_t cmp;
    const void *val;
    void *fnd;
} search_wrapper_arg_t;

// ========================================
// Funciones auxiliares
// ========================================

static Nodo newNodo(void *val)
{
    return val
               ? alloc(
                     struct Nodo,
                     &cast(
                         struct Nodo,
                         .der = NULL,
                         .val = val))
               : NULL;
}

static void *destroyNodo(Nodo nodo)
{
    void *val = NULL;

    if (nodo)
    {
        val = nodo->val;
        freem(nodo);
    }

    return val;
}

// ========================================
// Recorridos internos (Nodo*)
// ========================================

static void forEachInLista_Ref(Lista lista, operfn_t cb, void *arg)
{
    if (!lista || !cb)
        return;

    Nodo *ref = &cast(__Lista, lista)->start;
    int idx = 0;

    for (int idx = 0; ref; idx++)
    {
        switch (cb(arg, idx, ref))
        {
        case FOREACH_CONTINUE:
            ref = *ref ? &(*ref)->der : NULL;
            break;
        case FOREACH_STAY:
            break;
        case FOREACH_BREAK:
            return;
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
    if (!val || !arg)
        return FOREACH_BREAK;

    foreach_wrapper_arg_t *wrapper_arg = arg;

    if (!wrapper_arg->cb)
        return FOREACH_BREAK;

    return wrapper_arg->cb(
        wrapper_arg->arg,
        idx,
        *cast(Nodo *, val));
}

static new_operfn(foreach_wrapper_Lista_Value)
{
    if (!val || !arg)
        return FOREACH_BREAK;

    foreach_wrapper_arg_t *wrapper_arg = arg;

    if (!wrapper_arg->cb)
        return FOREACH_BREAK;

    return wrapper_arg->cb(
        wrapper_arg->arg,
        idx,
        cast(Nodo, val)->val);
}

// ========================================
// API Pública - Recorrido
// ========================================

static void forEachInLista_Nodo(Lista lista, operfn_t cb, void *arg)
{
    foreach_wrapper_arg_t wrapper_arg = {
        .cb = cb,
        .arg = arg};
    forEachInLista_Ref(lista, foreach_wrapper_Lista_Nodo, &wrapper_arg);
}

void forEachInLista(Lista lista, operfn_t cb, void *arg)
{
    foreach_wrapper_arg_t wrapper_arg = {
        .cb = cb,
        .arg = arg};
    forEachInLista_Nodo(lista, foreach_wrapper_Lista_Value, &wrapper_arg);
}

// ========================================
// API Pública - Creación
// ========================================

Lista newLista(cmpfn_t cmp)
{
    return alloc(
        struct __Lista,
        &cast(
            struct __Lista,
            .start = NULL,
            .cmp = cmp));
}

// ========================================
// API Pública - Inserción
// ========================================

static new_operfn(insert_wrapper_Lista)
{
    insert_wrapper_arg_t *wrapper_arg = arg;
    Nodo *ref = val;

    if (*ref && (!wrapper_arg->cmp ||
                 wrapper_arg->cmp((*ref)->val,
                                  wrapper_arg->nodo->val) < 0))
        return FOREACH_CONTINUE;

    wrapper_arg->nodo->der = *ref;
    *ref = wrapper_arg->nodo;
    *(wrapper_arg->idx) = idx;

    return FOREACH_BREAK;
}

int insertValueInLista(Lista lista, void *val)
{
    if (!lista)
        return INSERT_FAILED;

    Nodo nodo = newNodo(val);
    if (!nodo)
        return INSERT_FAILED;

    int idx = INSERT_FAILED;
    insert_wrapper_arg_t wrapper_arg = {
        .cmp = cast(__Lista, lista)->cmp,
        .nodo = nodo,
        .idx = &idx};

    forEachInLista_Ref(lista, insert_wrapper_Lista, &wrapper_arg);

    if (idx == INSERT_FAILED)
        destroyNodo(nodo);

    return idx;
}

// ========================================
// API Pública - Búsqueda
// ========================================

static new_operfn(search_wrapper_Lista)
{
    search_wrapper_arg_t *wrapper_arg = arg;

    if (wrapper_arg->cmp(val, wrapper_arg->val))
        return FOREACH_CONTINUE;

    wrapper_arg->fnd = val;
    return FOREACH_BREAK;
}

static new_operfn(search_wrapper_Lista_Ref)
{
    search_wrapper_arg_t *wrapper_arg = arg;

    if (wrapper_arg->cmp((*cast(Nodo *, val))->val, wrapper_arg->val))
        return FOREACH_CONTINUE;

    wrapper_arg->fnd = val;
    return FOREACH_BREAK;
}

void *searchValueInLista(Lista lista, const void *val, cmpfn_t cmp)
{
    if (!lista || !val || !cmp)
        return NULL;

    search_wrapper_arg_t wrapper_arg = {
        .cmp = cmp,
        .val = val,
        .fnd = NULL};

    forEachInLista(lista, search_wrapper_Lista, &wrapper_arg);
    return wrapper_arg.fnd;
}

static Nodo *searchValueInLista_Ref(Lista lista, const void *val, cmpfn_t cmp)
{
    if (!lista || !val || !cmp)
        return NULL;

    search_wrapper_arg_t wrapper_arg = {
        .cmp = cmp,
        .val = val,
        .fnd = NULL};

    forEachInLista_Ref(lista, search_wrapper_Lista_Ref, &wrapper_arg);
    return wrapper_arg.fnd;
}

// ========================================
// API Pública - Eliminación
// ========================================

void *deleteValueInLista(Lista lista, const void *val, cmpfn_t cmp)
{
    Nodo *ref = searchValueInLista_Ref(lista, val, cmp);
    if (!ref)
        return NULL;

    Nodo nodo = *ref;
    *ref = nodo->der;
    return destroyNodo(nodo);
}

// ========================================
// API Pública - Destrucción
// ========================================

static new_operfn(destroy_wrapper_Ref)
{
    if (!val)
        return FOREACH_BREAK;

    Nodo *ref = val;
    Nodo nodo = *ref;

    if (!nodo)
        return FOREACH_BREAK;

    foreach_wrapper_Lista_Value(arg, idx, nodo);

    *ref = nodo->der;
    destroyNodo(nodo);

    return FOREACH_STAY;
}

void destroyLista(Lista lista, operfn_t cb, void *arg)
{
    foreach_wrapper_arg_t wrapper_arg = {
        .cb = cb,
        .arg = arg};

    forEachInLista_Ref(lista, destroy_wrapper_Ref, &wrapper_arg);
    freem(lista);
}
