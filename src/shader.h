#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include "mat4x4.h"

typedef struct shader
{
  unsigned int id;
} shader_t;

shader_t shader_create(char *vertexPath, char *fragmentPath);

void shader_use(shader_t program);

void shader_setBool(shader_t program, char *name, bool value);

void shader_setInt(shader_t program, char *name, int value);

void shader_setFloat(shader_t program, char *name, float value);

void shader_setMat4x4(shader_t program, char *name, mat4x4_t mat);

#endif