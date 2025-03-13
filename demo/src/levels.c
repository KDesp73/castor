#include "levels.h"
#include "context.h"
#include "entity.h"
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

    AppendSprite(ctx, SpriteNew( 
        8.5, 2.5,
        SPRITE_DOWN,
        SPRITE_BARREL,
        2,
        true
    ));
    AppendSprite(ctx, SpriteNew(
        5.5, 10.5,
        SPRITE_UP,
        SPRITE_LAMP,
        2, 
        false
    ));
    AppendSprite(ctx, SpriteNew(
        4.5, 4.5,
        SPRITE_MIDDLE,
        SPRITE_COLUMN,
        2,
        true
    ));
    AppendEntity(ctx, EntityNew(
        SpriteNew(
            2.5, 2.5,
            SPRITE_DOWN,
            SPRITE_GOBLIN,
            3,
            false
        ),
        6.0f,
        100,
        0.5,
        0.5,
        10,
        10.0f,
        MoveSmoothAStar
    ));
    AppendSprite(ctx, SpriteNew(
        10.5, 10.5,
        SPRITE_DOWN,
        SPRITE_SKELETON,
        3,
        false
    ));
    AppendSprite(ctx, SpriteNew(
        14.5, 3.5,
        SPRITE_MIDDLE,
        SPRITE_EYE,
        3,
        false
    ));
    AppendSprite(ctx, SpriteNew(
        12.5, 12.5,
        SPRITE_DOWN,
        SPRITE_MUSHROOM,
        3,
        false
    ));

}

void Level3(Context* ctx)
{
    LoadLevelMap(ctx, "levels/3.lvl");
    
}

void Level4(Context* ctx)
{
    LoadLevelMap(ctx, "levels/4.lvl");
    
}

