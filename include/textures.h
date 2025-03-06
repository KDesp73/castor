#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL_render.h>
#include <stdbool.h>
SDL_Texture* LoadTexture(SDL_Renderer *renderer, const char* file_path);

bool LoadTextures(SDL_Renderer* renderer, SDL_Texture** textures, const char* path);

#endif // TEXTURES_H
