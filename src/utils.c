#include <stdlib.h>
#include <stdio.h>

char *utils_getFileContent(char *path)
{
  FILE *file = fopen(path, "r");
  if (file == NULL)
  {
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long len = ftell(file);
  char *content = malloc((len + 1) * sizeof(char));
  if (content == NULL)
  {
    fclose(file);
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_SET);
  fread(content, sizeof(char), len, file);
  fclose(file);

  content[len] = '\0';
  return content;
}

float clampf(float val, float lower, float upper)
{
  if (val < lower)
  {
    return lower;
  }
  if (val > upper)
  {
    return upper;
  }
  return val;
}