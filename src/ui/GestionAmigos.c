#include <ui/interfaces.h>
void AgregarAmigo(const char *username){
    Lista amigos = usuario->amigos; // Lista de amigos del usuario
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);
    if (amigo) {
        printf("El usuario %s ya es tu amigo.\n", amigo->username);
        return; // El usuario ya es amigo
    }else{
        amigo= searchValueInABB(usuarios, username, cmpUsuarioConUsername);
        insertValueInLista(amigos,amigo);
        printf("El usuario %s ha sido agregado a tu lista de amigos.\n", amigo->username);
    }
}
void EliminarAmigo(const char *username){
   Lista amigos=usuario->amigos; //Lista de amigos del usuario
    Usuario *amigo = searchValueInLista(amigos, username, cmpUsuarioConUsername);//Se busca que el usuername exista en la lista de amigos
    if (amigo) {
        deleteValueInLista(amigos,amigo->username, cmpUsuarioConUsername); //no realizamos free porque dentro de la funcion delete value se realiz el free del nodo
        printf("El usuario %s ha sido eliminado de tu lista de amigos.\n", amigo->username);
    } else {
        printf("El usuario %s no está en tu lista de amigos.\n", username);
    }
}
