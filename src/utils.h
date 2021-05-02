#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void *utils_malloc(size_t size);

int utils_getToken(char *str, char delim, char *token, char **tokenEnd);

char *utils_getFileContent(char *path);

float clampf(float val, float lower, float upper);

#endif