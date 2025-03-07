#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL_render.h>
#include <stdbool.h>

#define TEXTURES_LIST_FILE "textures.list"

SDL_Texture* TextureLoad(SDL_Renderer *renderer, const char* file_path);
bool TexturesLoad(SDL_Renderer* renderer, SDL_Texture** textures, const char* path);

#endif // TEXTURES_H
