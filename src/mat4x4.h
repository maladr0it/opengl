#ifndef MAT4X4_H
#define MAT4X4_H

#include "v3.h"

typedef struct mat4x4
{
    float m[4][4];
} mat4x4_t;

mat4x4_t mat4x4_mul(mat4x4_t a, mat4x4_t b);

mat4x4_t mat4x4_createIdentity(void);

mat4x4_t mat4x4_createScale(v3_t s);

mat4x4_t mat4x4_createRotX(float theta);

mat4x4_t mat4x4_createRotY(float theta);

mat4x4_t mat4x4_createRotZ(float theta);

mat4x4_t mat4x4_createTranslate(v3_t t);

mat4x4_t mat4x4_createProj(float aspectRatio, float fov, float zNear, float zFar);

mat4x4_t mat4x4_createLookAt(v3_t pos, v3_t target, v3_t up);

#endif