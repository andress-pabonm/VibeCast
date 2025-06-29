#include <utils/nodos/abb.h>
#include <utils/nodos/constants.h>
#include <utils/memmgr.h>
#include <utils/macros.h>
#include <stddef.h>

// ========================================
// Estructuras internas
// ========================================

typedef struct Nodo
{
    struct Nodo *l;
    struct Nodo *r;
    void *v;
} *Nodo;

typedef struct __ABB
{
    Nodo root;
    cmpfn_t cmp;
} *__ABB;

typedef struct
{
    operfn_t callback;
    void *arg;
} foreach_wrapper_arg_t;

// ========================================
// Funciones auxiliares
// ========================================

static Nodo newNodo(void *value_ptr)
{
    return value_ptr
               ? alloc(
                     struct Nodo,
                     &cast(struct Nodo,
                           .l = NULL,
                           .r = NULL,
                           .v = value_ptr))
               : NULL;
}

static void destroyNodo(Nodo nodo)
{
    freem(nodo);
}

// ========================================
// API Pública - Creación
// ========================================

ABB newABB(cmpfn_t cmp)
{
    return cmp ? alloc(struct __ABB, &cast(struct __ABB, .root = NULL, .cmp = cmp)) : NULL;
}

// ========================================
// API Pública - Inserción
// ========================================

int insertValueInABB(ABB abb, void *value_ptr)
{
    if (!abb)
        return INSERT_FAILED;
    Nodo nodo = newNodo(value_ptr);
    if (!nodo)
        return INSERT_FAILED;

    __ABB __abb = abb;
    Nodo *ref = &__abb->root;
    cmpfn_t cmp = __abb->cmp;
    int i = 0;

    while (*ref)
    {
        int cmpv = cmp((*ref)->v, value_ptr);
        if (!cmpv)
        {
            destroyNodo(nodo);
            return INSERT_FAILED;
        }
        i++;
        ref = (cmpv > 0) ? &(*ref)->l : &(*ref)->r;
    }

    *ref = nodo;
    return i;
}

// ========================================
// API Pública - Búsqueda
// ========================================

static Nodo *searchValueInABB_Ref(ABB abb, const void *value_ptr, cmpfn_t cmp)
{
    if (!abb || !value_ptr || !cmp)
        return NULL;

    Nodo *ref = &cast(__ABB, abb)->root;
    while (*ref)
    {
        int cmpv = cmp((*ref)->v, value_ptr);
        if (!cmpv)
            return ref;
        ref = (cmpv > 0) ? &(*ref)->l : &(*ref)->r;
    }
    return NULL;
}

static Nodo searchValueInABB_Nodo(ABB abb, const void *value_ptr, cmpfn_t cmp)
{
    Nodo *ref = searchValueInABB_Ref(abb, value_ptr, cmp);
    return ref ? *ref : NULL;
}

void *searchValueInABB(ABB abb, const void *value_ptr, cmpfn_t cmp)
{
    Nodo nodo = searchValueInABB_Nodo(abb, value_ptr, cmp);
    return nodo ? nodo->v : NULL;
}

// ========================================
// API Pública - Eliminación
// ========================================

static void *deleteValueInABB_Rec(Nodo *root)
{
    if (!root || !*root)
        return NULL;
    Nodo nodo = *root;
    void *value_ptr = nodo->v;

    if (nodo->l && nodo->r)
    {
        root = &nodo->r;
        while ((*root)->l)
            root = &(*root)->l;
        nodo->v = deleteValueInABB_Rec(root);
    }
    else
    {
        *root = nodo->l ? nodo->l : nodo->r;
        destroyNodo(nodo);
    }

    return value_ptr;
}

void *deleteValueInABB(ABB abb, const void *value_ptr, cmpfn_t cmp)
{
    return deleteValueInABB_Rec(searchValueInABB_Ref(abb, value_ptr, cmp));
}

// ========================================
// API Pública - Recorridos
// ========================================

static new_operfn(forEachInABB_PreOrder_Rec)
{
    if (!value_ptr)
        return FOREACH_CONTINUE;
    Nodo root = value_ptr;
    foreach_wrapper_arg_t *wrapper_arg = arg;

    switch (wrapper_arg->callback(wrapper_arg->arg, index, root->v))
    {
    case FOREACH_BREAK:
        return FOREACH_CONTINUE;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_PreOrder_Rec(arg, index + 1, root->l))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_PreOrder_Rec(arg, index + 1, root->r))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    return FOREACH_CONTINUE;
}

void forEachInABB_PreOrder(ABB abb, operfn_t callback, void *arg)
{
    if (!abb || !callback)
        return;
    foreach_wrapper_arg_t wrapper_arg = {.callback = callback, .arg = arg};
    forEachInABB_PreOrder_Rec(&wrapper_arg, 0, cast(__ABB, abb)->root);
}

static new_operfn(forEachInABB_InOrder_Rec)
{
    if (!value_ptr)
        return FOREACH_CONTINUE;
    Nodo root = value_ptr;
    foreach_wrapper_arg_t *wrapper_arg = arg;

    switch (forEachInABB_InOrder_Rec(arg, index + 1, root->l))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (wrapper_arg->callback(wrapper_arg->arg, index, root->v))
    {
    case FOREACH_BREAK:
        return FOREACH_CONTINUE;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_InOrder_Rec(arg, index + 1, root->r))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    return FOREACH_CONTINUE;
}

void forEachInABB_InOrder(ABB abb, operfn_t callback, void *arg)
{
    if (!abb || !callback)
        return;
    foreach_wrapper_arg_t wrapper_arg = {.callback = callback, .arg = arg};
    forEachInABB_InOrder_Rec(&wrapper_arg, 0, cast(__ABB, abb)->root);
}

static new_operfn(forEachInABB_PostOrder_Rec)
{
    if (!value_ptr)
        return FOREACH_CONTINUE;
    Nodo root = value_ptr;
    foreach_wrapper_arg_t *wrapper_arg = arg;

    switch (forEachInABB_PostOrder_Rec(arg, index + 1, root->l))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_PostOrder_Rec(arg, index + 1, root->r))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (wrapper_arg->callback(wrapper_arg->arg, index, root->v))
    {
    case FOREACH_BREAK:
        return FOREACH_CONTINUE;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    return FOREACH_CONTINUE;
}

void forEachInABB_PostOrder(ABB abb, operfn_t callback, void *arg)
{
    if (!abb || !callback)
        return;
    foreach_wrapper_arg_t wrapper_arg = {.callback = callback, .arg = arg};
    forEachInABB_PostOrder_Rec(&wrapper_arg, 0, cast(__ABB, abb)->root);
}

// ========================================
// API Pública - Destrucción
// ========================================

void destroyABB_Rec_NoCallback(Nodo root)
{
    if (root)
    {
        destroyABB_Rec_NoCallback(root->l);
        destroyABB_Rec_NoCallback(root->r);
        destroyNodo(root);
    }
}

void destroyABB_Rec_Callback(Nodo root, operfn_t callback, void *arg)
{
    if (root)
    {
        destroyABB_Rec_Callback(root->l, callback, arg);
        destroyABB_Rec_Callback(root->r, callback, arg);
        callback(arg, 0, root->v);
        destroyNodo(root);
    }
}

void destroyABB(ABB abb, operfn_t callback, void *arg)
{
    if (!abb)
        return;

    __ABB __abb = abb;
    if (callback)
        destroyABB_Rec_Callback(__abb->root, callback, arg);
    else
        destroyABB_Rec_NoCallback(__abb->root);
}