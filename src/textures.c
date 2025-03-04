#include "textures.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

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

