#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <glad/glad.h>
#include "mesh.h"
#include "utils.h"

static const int DIFFUSE_TEXTURES_OFFSET = 0;
static const int SPECULAR_TEXTURES_OFFSET = 3;
static char *TEXTURE_NAMES[] = {
    "diffuse1",
    "diffuse2",
    "diffuse3",
    "specular1",
    "specular2",
};

mesh_t mesh_load(char *path, texture_t *textures, int texturesLen)
{
  FILE *file = fopen(path, "r");
  if (file == NULL)
  {
    printf("failed to open file: %s", path);
    exit(EXIT_FAILURE);
  }

  // TODO: allocate only as much room as we need
  // count the number of faces and calculate how many verts will be created
  vertex_t *verts = utils_malloc(sizeof(vertex_t) * 128);
  int vertsLen = 0;

  v3_t *vertPositions = utils_malloc(sizeof(v3_t) * 128);
  int vertPositionsLen = 0;
  v2_t *vertTexCoords = utils_malloc(sizeof(v2_t) * 128);
  int vertTexCoordsLen = 0;

  char line[256];
  char token[64];

  while (fgets(line, 128, file) != NULL)
  {
    char *linePtr = line;
    utils_getToken(linePtr, ' ', token, &linePtr);

    if (strcmp(token, "v") == 0)
    {
      float posX = strtof(linePtr, &linePtr);
      float posY = strtof(linePtr, &linePtr);
      float posZ = strtof(linePtr, &linePtr);
      vertPositions[vertPositionsLen] = v3_create(posX, posY, posZ);
      ++vertPositionsLen;
    }
    else if (strcmp(token, "vt") == 0)
    {
      float texCoordX = strtof(linePtr, &linePtr);
      float texCoordY = strtof(linePtr, &linePtr);
      vertTexCoords[vertTexCoordsLen] = v2_create(texCoordX, texCoordY);
      vertTexCoordsLen++;
    }
    else if (strcmp(token, "f") == 0)
    {
      // TODO: rethink name for a 'sub-token' and the pointer within it
      char *tokenPtr;
      char indexStr[16];

      // vertex 1
      utils_getToken(linePtr, ' ', token, &linePtr);
      tokenPtr = token;
      utils_getToken(tokenPtr, '/', indexStr, &tokenPtr);
      int v1PositionIdx = strtol(indexStr, NULL, 10);
      utils_getToken(tokenPtr, '/', indexStr, &tokenPtr);
      int v1TexCoordIdx = strtol(indexStr, NULL, 10);

      // vertex 2
      utils_getToken(linePtr, ' ', token, &linePtr);
      tokenPtr = token;
      utils_getToken(tokenPtr, '/', indexStr, &tokenPtr);
      int v2PositionIdx = strtol(indexStr, NULL, 10);
      utils_getToken(tokenPtr, '/', indexStr, &tokenPtr);
      int v2TexCoordIdx = strtol(indexStr, NULL, 10);

      // vertex 3
      utils_getToken(linePtr, ' ', token, &linePtr);
      tokenPtr = token;
      utils_getToken(tokenPtr, '/', indexStr, &tokenPtr);
      int v3PositionIdx = strtol(indexStr, NULL, 10);
      utils_getToken(tokenPtr, '/', indexStr, &tokenPtr);
      int v3TexCoordIdx = strtol(indexStr, NULL, 10);

      vertex_t vert1;
      vert1.pos = vertPositions[v1PositionIdx];
      vert1.texCoords = vertTexCoords[v1TexCoordIdx];

      vertex_t vert2;
      vert2.pos = vertPositions[v2PositionIdx];
      vert2.texCoords = vertTexCoords[v2TexCoordIdx];

      vertex_t vert3;
      vert3.pos = vertPositions[v3PositionIdx];
      vert3.texCoords = vertTexCoords[v3TexCoordIdx];

      // assume CCW winding
      v3_t faceNormal = v3_cross(v3_sub(vert2.pos, vert1.pos), v3_sub(vert3.pos, vert1.pos));
      vert1.normal = faceNormal;
      vert2.normal = faceNormal;
      vert3.normal = faceNormal;

      verts[vertsLen++] = vert1;
      verts[vertsLen++] = vert2;
      verts[vertsLen++] = vert3;
    }
  }

  mesh_t mesh;
  mesh.vertices = verts;
  mesh.verticesLen = vertsLen;
  mesh.textures = textures;
  mesh.texturesLen = texturesLen;

  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh.verticesLen * sizeof(*mesh.vertices), mesh.vertices, GL_STATIC_DRAW);

  // vertex positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*mesh.vertices), (void *)offsetof(vertex_t, pos));
  glEnableVertexAttribArray(0);
  // vertex normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(*mesh.vertices), (void *)offsetof(vertex_t, normal));
  glEnableVertexAttribArray(1);
  // vertex texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(*mesh.vertices), (void *)offsetof(vertex_t, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  return mesh;
}

mesh_t mesh_create(
    vertex_t *vertices, int verticesLen,
    unsigned int *indices, int indicesLen,
    texture_t *textures, int texturesLen)
{
  mesh_t mesh;
  mesh.vertices = vertices;
  mesh.verticesLen = verticesLen;
  mesh.indices = indices;
  mesh.indicesLen = indicesLen;
  mesh.textures = textures;
  mesh.texturesLen = texturesLen;

  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);

  glBindVertexArray(mesh.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh.verticesLen * sizeof(*mesh.vertices), mesh.vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indicesLen * sizeof(*mesh.indices), mesh.indices, GL_STATIC_DRAW);

  // vertex positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*mesh.vertices), (void *)offsetof(vertex_t, pos));
  glEnableVertexAttribArray(0);
  // vertex normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(*mesh.vertices), (void *)offsetof(vertex_t, normal));
  glEnableVertexAttribArray(1);
  // vertex texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(*mesh.vertices), (void *)offsetof(vertex_t, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  return mesh;
}

void mesh_render(mesh_t mesh, shader_t shader)
{
  // set textures
  unsigned int numDiffuseMaps = 0;
  unsigned int numSpecularMaps = 0;

  for (int i = 0; i < mesh.texturesLen; ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);

    enum texture_type type = mesh.textures[i].type;

    if (type == DIFFUSE)
    {
      char *textureName = TEXTURE_NAMES[DIFFUSE_TEXTURES_OFFSET + numDiffuseMaps];
      shader_setInt(shader, textureName, i);
      ++numDiffuseMaps;
    }
    else if (type == SPECULAR)
    {
      char *textureName = TEXTURE_NAMES[SPECULAR_TEXTURES_OFFSET + numSpecularMaps];
      shader_setInt(shader, textureName, i);
      ++numSpecularMaps;
    }

    glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
  }

  glActiveTexture(GL_TEXTURE0);

  // render
  glBindVertexArray(mesh.VAO);
  glDrawElements(GL_TRIANGLES, mesh.indicesLen, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}