#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "context.h"
#include <SDL2/SDL_render.h>

#define TILE_EMPTY 0

void CastWalls(SDL_Renderer *renderer, Context* ctx);
void CastFloorAndCeiling(SDL_Renderer *renderer, const Context* ctx);
void CastSprites(SDL_Renderer* renderer, Context* ctx);



#endif // RAYCASTER_H

