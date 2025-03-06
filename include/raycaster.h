#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "context.h"
#include <SDL2/SDL_render.h>

typedef enum {
    TILE_EMPTY = 0,
    TILE_WHITE = 1,
    TILE_BLACK = 2,
    TILE_RED = 3,
    TILE_BLUE = 4,
    TILE_GREEN = 5,
} Tile;

void CastRays(SDL_Renderer *renderer, const Context* ctx);
void DrawFloorAndCeiling(SDL_Renderer *renderer, const Context* ctx);


#endif // RAYCASTER_H

