#include <glad/glad.h>
#include <stddef.h>
#include "mesh.h"

static const int DIFFUSE_TEXTURES_OFFSET = 0;
static const int SPECULAR_TEXTURES_OFFSET = 3;
static char *TEXTURE_NAMES[] = {
    "diffuse1",
    "diffuse2",
    "diffuse3",
    "specular1",
    "specular2",
};

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