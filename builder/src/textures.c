#include "textures.h"
#include <stdio.h>
#include <string.h>

bool LoadTextures(Texture2D textures[], const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open texture file");
        return false;
    }

    char line[MAX_PATH];
    size_t index = 0;

    while (fgets(line, sizeof(line), file) && index < MAX_TEXTURES) {
        // Trim leading spaces
        while (*line == ' ' || *line == '\t') memmove(line, line + 1, strlen(line));

        // Ignore empty lines and comments
        if (*line == '\n' || *line == '\r' || *line == '\0' || line[0] == '#') continue;

        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;

        // Load texture
        textures[index] = LoadTexture(line);
        if (textures[index].id == 0) {
            fprintf(stderr, "Error loading texture from path: %s\n", line);
            continue;
        }

        printf("Loaded texture %zu: %s\n", index, line);
        index++;
    }

    fclose(file);
    return true;
}
