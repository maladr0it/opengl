#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

void *utils_malloc(size_t size)
{
    void *result = malloc(size);
    if (result == NULL)
    {
        printf("failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    return result;
}

int utils_getToken(char *str, char delim, char *token, char **tokenEnd)
{
    int tokenLength = 0;
    char *p = str;

    while (*p == delim && *p != '\0' && *p != '\n')
    {
        ++p;
    }

    while (*p != delim && *p != '\0' && *p != '\n')
    {
        token[tokenLength] = *p;
        ++tokenLength;
        ++p;
    }

    token[tokenLength] = '\0';
    *tokenEnd = p;
    return tokenLength;
}

char *utils_getFileContent(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("failed to open file: %s", path);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    char *content = utils_malloc((len + 1) * sizeof(char));

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
    else if (val > upper)
    {
        return upper;
    }
    else
    {
        return val;
    }
}