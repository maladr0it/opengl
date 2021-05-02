#ifndef MESH_H
#define MESH_H

#include "v2.h"
#include "v3.h"
#include "texture.h"
#include "shader.h"

typedef struct vertex
{
  v3_t pos;
  v3_t normal;
  v2_t texCoords;
} vertex_t;

typedef struct mesh
{
  vertex_t *vertices;
  int verticesLen;
  unsigned int *indices;
  int indicesLen;
  texture_t *textures;
  int texturesLen;

  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
} mesh_t;

mesh_t mesh_load(char *path, texture_t *textures, int texturesLen);

mesh_t mesh_create(
    vertex_t *vertices, int verticesLen,
    unsigned int *indices, int indicesLen,
    texture_t *textures, int texturesLen);

void mesh_render(mesh_t mesh, shader_t shader);

#endif