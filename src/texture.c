#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stdbool.h>
#include "texture.h"

texture_t texture_load(char *path, enum texture_type type)
{
  texture_t texture;
  texture.type = type;
  glGenTextures(1, &texture.id);

  int width, height, numComponents;
  unsigned char *data = stbi_load(path, &width, &height, &numComponents, 0);

  if (data == NULL)
  {
    printf("Failed to load image %s", path);
    exit(EXIT_FAILURE);
  }

  GLenum format = 0;
  if (numComponents == 1)
  {
    format = GL_RED;
  }
  else if (numComponents == 3)
  {
    format = GL_RGB;
  }
  else if (numComponents == 4)
  {
    format = GL_RGBA;
  }

  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  return texture;
}