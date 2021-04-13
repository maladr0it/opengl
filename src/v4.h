#ifndef V4_H
#define V4_H

typedef struct v4
{
  float x;
  float y;
  float z;
  float w;
} v4_t;

v4_t v4_create(float x, float y, float z, float w);

#endif