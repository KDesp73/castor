#include "textures.h"
#include "context.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

SDL_Texture* LoadTexture(SDL_Renderer *renderer, const char* file_path)
{
    SDL_Surface* surface = IMG_Load(file_path);
    if (!surface) {
        fprintf(stderr, "Failed to load texture: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

#define MAX_PATH 256

bool LoadTextures(SDL_Renderer* renderer, SDL_Texture** textures, const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open texture file");
        return false;
    }

    char line[MAX_PATH + 10];
    size_t index = 1;

    while (fgets(line, sizeof(line), file) && index < MAX_TEXTURES) {
        // Trim leading spaces
        size_t offset = strspn(line, " \t");

        // Ignore empty lines & comments
        if (line[offset] == '#' || line[offset] == '\n' || line[offset] == '\r' || line[offset] == '\0')
            continue;

        // Remove trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        // Load texture
        textures[index] = LoadTexture(renderer, line + offset);
        if (!textures[index]) {
            fprintf(stderr, "Error loading texture: %s\n", line + offset);
        } else {
            printf("Loaded texture %zu: %s\n", index, line + offset);
            index++;
        }
    }

    fclose(file);
    return true;
}
