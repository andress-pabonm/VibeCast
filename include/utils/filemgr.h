#ifndef VIBECAST_FILEMGR_H
#define VIBECAST_FILEMGR_H 1

#include <stdio.h>

FILE *newFile(const char *filename, char **errmsg);
FILE *getFile(const char *filename, char **errmsg);
char *readFile(const char *filename, char **errmsg);

#endif // VIBECAST_FILEMGR_H