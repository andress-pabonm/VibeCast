#ifndef VIBECAST_INTERFACES_H
#define VIBECAST_INTERFACES_H

/* Aquí van las declaraciones de las funciones principales de cada interfaz */

void crearPlaylist(const char *nombrePlaylist, const char *fechaCreacion, Lista *caniones);
bool agregarCancion(const char *nombrePlaylist, const char *nombreCancion);
#endif // VIBECAST_INTERFACES_H