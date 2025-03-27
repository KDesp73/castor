#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL_render.h>
#include <stdbool.h>

#define TEXTURES_LIST_FILE "assets/textures.list"
#define SPRITES_LIST_FILE "assets/sprites.list"

SDL_Texture* castor_TextureLoad(SDL_Renderer *renderer, const char* file_path);
bool castor_TexturesLoad(SDL_Renderer* renderer, SDL_Texture** textures, const char* path);

#endif // TEXTURES_H
