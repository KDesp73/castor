#include "levels.h"
#include "context.h"

enum {
    SPRITE_BARREL,
    SPRITE_COLUMN,
    SPRITE_LAMP
};

void Level2(Context* ctx)
{
    LoadLevelMap(ctx, "levels/2.lvl");

    AppendSprite(ctx, (Sprite) {
        .x = 8.5,
        .y = 2.5,
        .z = -0.5,
        .texture_id = SPRITE_BARREL,
        .width = 64,
        .height = 64,
        .distance = 1,
        .scale = 3
    });
    AppendSprite(ctx, (Sprite) {
        .x = 5.5,
        .y = 10.5,
        .z = 0.5,
        .texture_id = SPRITE_LAMP,
        .width = 64,
        .height = 64,
        .distance = 0,
        .scale = 2
    });

}

