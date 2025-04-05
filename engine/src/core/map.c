#include "core.h"

castor_Map* castor_ExportSearchMap(castor_Context* ctx)
{
    castor_Map* res = castor_MapCreate(ctx->level.map->h, ctx->level.map->w);
    if (!res) return NULL;

    // Fill based on original map
    for (size_t j = 0; j < ctx->level.map->h; j++) {
        for (size_t i = 0; i < ctx->level.map->w; i++) {
            res->grid[j][i] = (ctx->level.map->grid[j][i] != 0) ? 1 : 0;
        }
    }

    // Mark sprite collisions
    for (size_t k = 0; k < ctx->level.sprite_count; k++) {
        castor_Sprite* sprite = ctx->level.sprites[k];
        if(!sprite) continue;

        if (sprite->collision) {
            int gridX = (int)floor(sprite->x);
            int gridY = (int)floor(sprite->y);

            if (gridX >= 0 && gridX < ctx->level.map->w && gridY >= 0 && gridY < ctx->level.map->h) {
                res->grid[gridY][gridX] = 1;
            }
        }
    }

    return res;
}


void castor_FreeLevel(castor_Context* ctx)
{
    castor_FreeSprites(ctx);
    castor_MapFree(&ctx->level.map);
    castor_FreeEntities(ctx);
    castor_FreeEvents(ctx);
    castor_FreeItems(ctx);
}


void castor_LoadLevelMap(castor_Context* ctx, const char* path)
{
    ctx->level.map = castor_MapLoad(path);
}
