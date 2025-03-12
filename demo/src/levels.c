#include "levels.h"
#include "context.h"
#include "sprite.h"

enum {
    SPRITE_BARREL,
    SPRITE_COLUMN,
    SPRITE_LAMP,
    SPRITE_GOBLIN,
    SPRITE_SKELETON,
    SPRITE_EYE,
    SPRITE_MUSHROOM
};

void Level1(Context* ctx)
{
    LoadLevelMap(ctx, "levels/1.lvl");
    
}

void Level2(Context* ctx)
{
    LoadLevelMap(ctx, "levels/2.lvl");

    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(8, 2),
        .z = SPRITE_DOWN,
        .texture_id = SPRITE_BARREL,
        .scale = 2,
        .collision = true,
    });
    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(5, 10),
        .z = SPRITE_UP,
        .texture_id = SPRITE_LAMP,
        .scale = 2
    });
    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(4, 4),
        .z = SPRITE_MIDDLE,
        .texture_id = SPRITE_COLUMN,
        .scale = 2,
        .collision = true
    });
    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(2, 2),
        .z = SPRITE_DOWN,
        .texture_id = SPRITE_GOBLIN,
        .scale = 3,
    });
    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(10, 10),
        .z = SPRITE_DOWN,
        .texture_id = SPRITE_SKELETON,
        .scale = 3,
    });
    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(14, 3),
        .z = SPRITE_MIDDLE,
        .texture_id = SPRITE_EYE,
        .scale = 3,
    });
    AppendSprite(ctx, (Sprite) {
        SPRITE_PLACE(12, 12),
        .z = SPRITE_DOWN,
        .texture_id = SPRITE_MUSHROOM,
        .scale = 3,
    });

}

void Level3(Context* ctx)
{
    LoadLevelMap(ctx, "levels/3.lvl");
    
}

void Level4(Context* ctx)
{
    LoadLevelMap(ctx, "levels/4.lvl");
    
}

