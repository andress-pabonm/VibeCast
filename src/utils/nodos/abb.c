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
    operfn_t cb;
    void *arg;
} foreach_wrapper_arg_t;

// ========================================
// Funciones auxiliares
// ========================================

static Nodo newNodo(void *val)
{
    return val
               ? alloc(
                     struct Nodo,
                     &cast(struct Nodo,
                           .l = NULL,
                           .r = NULL,
                           .v = val))
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

int insertValueInABB(ABB abb, void *val)
{
    if (!abb)
        return INSERT_FAILED;
    Nodo nodo = newNodo(val);
    if (!nodo)
        return INSERT_FAILED;

    __ABB __abb = abb;
    Nodo *ref = &__abb->root;
    cmpfn_t cmp = __abb->cmp;
    int i = 0;

    while (*ref)
    {
        int cmpv = cmp((*ref)->v, val);
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

static Nodo *searchValueInABB_Ref(ABB abb, const void *val, cmpfn_t cmp)
{
    if (!abb || !val || !cmp)
        return NULL;

    Nodo *ref = &cast(__ABB, abb)->root;
    while (*ref)
    {
        int cmpv = cmp((*ref)->v, val);
        if (!cmpv)
            return ref;
        ref = (cmpv > 0) ? &(*ref)->l : &(*ref)->r;
    }
    return NULL;
}

static Nodo searchValueInABB_Nodo(ABB abb, const void *val, cmpfn_t cmp)
{
    Nodo *ref = searchValueInABB_Ref(abb, val, cmp);
    return ref ? *ref : NULL;
}

void *searchValueInABB(ABB abb, const void *val, cmpfn_t cmp)
{
    Nodo nodo = searchValueInABB_Nodo(abb, val, cmp);
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
    void *val = nodo->v;

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

    return val;
}

void *deleteValueInABB(ABB abb, const void *val, cmpfn_t cmp)
{
    return deleteValueInABB_Rec(searchValueInABB_Ref(abb, val, cmp));
}

// ========================================
// API Pública - Recorridos
// ========================================

static new_operfn(forEachInABB_PreOrder_Rec)
{
    if (!val)
        return FOREACH_CONTINUE;
    Nodo root = val;
    foreach_wrapper_arg_t *wrapper_arg = arg;

    switch (wrapper_arg->cb(wrapper_arg->arg, idx, root->v))
    {
    case FOREACH_BREAK:
        return FOREACH_CONTINUE;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_PreOrder_Rec(arg, idx + 1, root->l))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_PreOrder_Rec(arg, idx + 1, root->r))
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

void forEachInABB_PreOrder(ABB abb, operfn_t cb, void *arg)
{
    if (!abb || !cb)
        return;
    foreach_wrapper_arg_t wrapper_arg = {.cb = cb, .arg = arg};
    forEachInABB_PreOrder_Rec(&wrapper_arg, 0, cast(__ABB, abb)->root);
}

static new_operfn(forEachInABB_InOrder_Rec)
{
    if (!val)
        return FOREACH_CONTINUE;
    Nodo root = val;
    foreach_wrapper_arg_t *wrapper_arg = arg;

    switch (forEachInABB_InOrder_Rec(arg, idx + 1, root->l))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (wrapper_arg->cb(wrapper_arg->arg, idx, root->v))
    {
    case FOREACH_BREAK:
        return FOREACH_CONTINUE;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_InOrder_Rec(arg, idx + 1, root->r))
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

void forEachInABB_InOrder(ABB abb, operfn_t cb, void *arg)
{
    if (!abb || !cb)
        return;
    foreach_wrapper_arg_t wrapper_arg = {.cb = cb, .arg = arg};
    forEachInABB_InOrder_Rec(&wrapper_arg, 0, cast(__ABB, abb)->root);
}

static new_operfn(forEachInABB_PostOrder_Rec)
{
    if (!val)
        return FOREACH_CONTINUE;
    Nodo root = val;
    foreach_wrapper_arg_t *wrapper_arg = arg;

    switch (forEachInABB_PostOrder_Rec(arg, idx + 1, root->l))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (forEachInABB_PostOrder_Rec(arg, idx + 1, root->r))
    {
    case FOREACH_BREAK:
        return FOREACH_BREAK;
    case FOREACH_CONTINUE:
        break;
    default:
        return FOREACH_BREAK;
    }

    switch (wrapper_arg->cb(wrapper_arg->arg, idx, root->v))
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

void forEachInABB_PostOrder(ABB abb, operfn_t cb, void *arg)
{
    if (!abb || !cb)
        return;
    foreach_wrapper_arg_t wrapper_arg = {.cb = cb, .arg = arg};
    forEachInABB_PostOrder_Rec(&wrapper_arg, 0, cast(__ABB, abb)->root);
}

// ========================================
// API Pública - Destrucción
// ========================================

void destroyABB_Rec_Nocb(Nodo root)
{
    if (root)
    {
        destroyABB_Rec_Nocb(root->l);
        destroyABB_Rec_Nocb(root->r);
        destroyNodo(root);
    }
}

void destroyABB_Rec_cb(Nodo root, operfn_t cb, void *arg)
{
    if (root)
    {
        destroyABB_Rec_cb(root->l, cb, arg);
        destroyABB_Rec_cb(root->r, cb, arg);
        cb(arg, 0, root->v);
        destroyNodo(root);
    }
}

void destroyABB(ABB abb, operfn_t cb, void *arg)
{
    if (!abb)
        return;

    __ABB __abb = abb;
    if (cb)
        destroyABB_Rec_cb(__abb->root, cb, arg);
    else
        destroyABB_Rec_Nocb(__abb->root);
}