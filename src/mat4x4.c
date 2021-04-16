#include <stdlib.h>
#include <math.h>
#include "mat4x4.h"

mat4x4_t mat4x4_mul(mat4x4_t a, mat4x4_t b)
{
    mat4x4_t result;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.m[row][col] = a.m[row][0] * b.m[0][col] + a.m[row][1] * b.m[1][col] + a.m[row][2] * b.m[2][col] + a.m[row][3] * b.m[3][col];
        }
    }

    return result;
}

v4_t mat4x4_mulv4(mat4x4_t mat, v4_t v)
{
    v4_t result;

    result.x = mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3] * v.w;
    result.y = mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3] * v.w;
    result.z = mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * v.w;
    result.w = mat.m[3][0] * v.x + mat.m[3][1] * v.y + mat.m[3][2] * v.z + mat.m[3][3] * v.w;

    return result;
}

mat4x4_t mat4x4_createIdentity()
{
    mat4x4_t result;

    result.m[0][0] = 1;
    result.m[0][1] = 0;
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = 1;
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = 0;
    result.m[2][2] = 1;
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;

    return result;
}

mat4x4_t mat4x4_createScale(v3_t s)
{
    mat4x4_t result;

    result.m[0][0] = s.x;
    result.m[0][1] = 0;
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = s.y;
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = 0;
    result.m[2][2] = s.z;
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;

    return result;
}

mat4x4_t mat4x4_createRotX(float theta)
{
    mat4x4_t result;

    result.m[0][0] = 1;
    result.m[0][1] = 0;
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = cosf(theta);
    result.m[1][2] = -sinf(theta);
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = sinf(theta);
    result.m[2][2] = cosf(theta);
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;

    return result;
}

mat4x4_t mat4x4_createRotY(float theta)
{
    mat4x4_t result;

    result.m[0][0] = cosf(theta);
    result.m[0][1] = 0;
    result.m[0][2] = sinf(theta);
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = 1;
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = -sinf(theta);
    result.m[2][1] = 0;
    result.m[2][2] = cosf(theta);
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;

    return result;
}

mat4x4_t mat4x4_createRotZ(float theta)
{
    mat4x4_t result;

    result.m[0][0] = cosf(theta);
    result.m[0][1] = -sinf(theta);
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = sinf(theta);
    result.m[1][1] = cosf(theta);
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = 0;
    result.m[2][2] = 1;
    result.m[2][3] = 0;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;

    return result;
}

mat4x4_t mat4x4_createTranslate(v3_t t)
{
    mat4x4_t result;

    result.m[0][0] = 1;
    result.m[0][1] = 0;
    result.m[0][2] = 0;
    result.m[0][3] = t.x;

    result.m[1][0] = 0;
    result.m[1][1] = 1;
    result.m[1][2] = 0;
    result.m[1][3] = t.y;

    result.m[2][0] = 0;
    result.m[2][1] = 0;
    result.m[2][2] = 1;
    result.m[2][3] = t.z;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = 0;
    result.m[3][3] = 1;

    return result;
}

// todo: probably need to swap rows/cols for below functions
mat4x4_t mat4x4_createProj(float aspectRatio, float fov, float zNear, float zFar)
{
    mat4x4_t result;

    float f = 1 / tan(fov / 2);
    float q = zFar / (zFar - zNear);

    result.m[0][0] = f * 1 / aspectRatio;
    result.m[0][1] = 0;
    result.m[0][2] = 0;
    result.m[0][3] = 0;

    result.m[1][0] = 0;
    result.m[1][1] = f;
    result.m[1][2] = 0;
    result.m[1][3] = 0;

    result.m[2][0] = 0;
    result.m[2][1] = 0;
    result.m[2][2] = q;
    result.m[2][3] = 1;

    result.m[3][0] = 0;
    result.m[3][1] = 0;
    result.m[3][2] = -zNear * q;
    result.m[3][3] = 0;

    return result;
}

mat4x4_t mat4x4_createPointAt(v3_t pos, v3_t target, v3_t up)
{
    v3_t newForward = v3_normalize(v3_sub(target, pos));
    v3_t a = v3_mul(newForward, v3_dot(up, newForward));
    v3_t newUp = v3_normalize(v3_sub(up, a));
    v3_t newRight = v3_cross(newForward, newUp);

    mat4x4_t result;

    result.m[0][0] = newRight.x;
    result.m[0][1] = newRight.y;
    result.m[0][2] = newRight.z;
    result.m[0][3] = 0;

    result.m[1][0] = -newUp.x;
    result.m[1][1] = -newUp.y;
    result.m[1][2] = -newUp.z;
    result.m[1][3] = 0;

    result.m[2][0] = newForward.x;
    result.m[2][1] = newForward.y;
    result.m[2][2] = newForward.z;
    result.m[2][3] = 0;

    result.m[3][0] = pos.x;
    result.m[3][1] = pos.y;
    result.m[3][2] = pos.z;
    result.m[3][3] = 1;

    return result;
}

mat4x4_t mat4x4_createLookAt(mat4x4_t pointAt)
{
    mat4x4_t result;

    result.m[0][0] = pointAt.m[0][0];
    result.m[0][1] = pointAt.m[1][0];
    result.m[0][2] = pointAt.m[2][0];
    result.m[0][3] = 0;

    result.m[1][0] = pointAt.m[0][1];
    result.m[1][1] = pointAt.m[1][1];
    result.m[1][2] = pointAt.m[2][1];
    result.m[1][3] = 0;

    result.m[2][0] = pointAt.m[0][2];
    result.m[2][1] = pointAt.m[1][2];
    result.m[2][2] = pointAt.m[2][2];
    result.m[2][3] = 0;

    result.m[3][0] = -(pointAt.m[3][0] * pointAt.m[0][0] + pointAt.m[3][1] * pointAt.m[0][1] + pointAt.m[3][2] * pointAt.m[0][2]);
    result.m[3][1] = -(pointAt.m[3][0] * pointAt.m[1][0] + pointAt.m[3][1] * pointAt.m[1][1] + pointAt.m[3][2] * pointAt.m[1][2]);
    result.m[3][2] = -(pointAt.m[3][0] * pointAt.m[2][0] + pointAt.m[3][1] * pointAt.m[2][1] + pointAt.m[3][2] * pointAt.m[2][2]);
    result.m[3][3] = 1;

    return result;
}