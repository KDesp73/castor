#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "core.h"
#include <SDL2/SDL_render.h>

#define TILE_EMPTY 0

void castor_CastWalls(SDL_Renderer *renderer, castor_Context* ctx);
void castor_CastFloorAndCeiling(SDL_Renderer *renderer, const castor_Context* ctx);
void castor_CastSprites(SDL_Renderer* renderer, castor_Context* ctx);
void castor_ApplyDarkenFilter(SDL_Renderer *renderer, int screen_width, int screen_height, Uint8 opacity);



#endif // RAYCASTER_H

