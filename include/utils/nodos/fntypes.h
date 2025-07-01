#ifndef VIBECAST_NODOS_FNTYPES_H
#define VIBECAST_NODOS_FNTYPES_H 1

/* ================ Tipos para funciones callback ================ */

// Para comparar dos valores en una colección
typedef int (*cmpfn_t)(const void *val_1, const void *value_ptr_2);
#define new_cmpfn(cmpfn_name) int cmpfn_name(const void *val_1, const void *val_2)

// Para recorrer una colección
typedef int (*operfn_t)(void *arg, int idx, void *val);
#define new_operfn(operfn_name) int operfn_name(void *arg, int idx, void *val)

#endif // VIBECAST_NODOS_FNTYPES_H
