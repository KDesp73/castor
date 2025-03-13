#include "levels.h"
#include "context.h"
#include "entity.h"
#include "map.h"
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

#define ENTITY_GOBLIN(x, y)   \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_GOBLIN,    \
            3,                \
            false             \
        ),                    \
        6.0f,                 \
        100,                  \
        0.5,                  \
        0.5,                  \
        4,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )
#define ENTITY_SKELETON(x, y) \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_SKELETON,  \
            3,                \
            false             \
        ),                    \
        7.0f,                 \
        100,                  \
        0.7,                  \
        0.4,                  \
        4,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )
#define ENTITY_MUSHROOM(x, y) \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_MUSHROOM,  \
            3,                \
            false             \
        ),                    \
        3.0f,                 \
        100,                  \
        0.3,                  \
        0.4,                  \
        6,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )
#define ENTITY_EYE(x, y)      \
    EntityNew(                \
        SpriteNew(            \
            x+0.5, y+0.5,     \
            SPRITE_DOWN,      \
            SPRITE_EYE,       \
            3,                \
            false             \
            ),                \
        5.0f,                 \
        100,                  \
        0.4,                  \
        0.6,                  \
        5,                    \
        10.0f,                \
        MoveSmoothAStar       \
    )

#define SPRITE_COLUMN(x, y)   \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_MIDDLE,        \
        SPRITE_COLUMN,        \
        2,                    \
        true                  \
    )
#define SPRITE_LAMP(x, y)     \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_UP,            \
        SPRITE_LAMP,          \
        2,                    \
        false                 \
    )
#define SPRITE_BARREL(x, y)   \
    SpriteNew(                \
        x+0.5, y+0.5,         \
        SPRITE_DOWN,          \
        SPRITE_BARREL,        \
        2,                    \
        true                  \
    )


void Level1(Context* ctx)
{
    LoadLevelMap(ctx, "levels/1.lvl");
    
    AppendSprite(ctx, SPRITE_COLUMN(9, 9));
}

void Level2(Context* ctx)
{
    LoadLevelMap(ctx, "levels/2.lvl");
    ctx->render_distance = 50;

    AppendSprite(ctx, SPRITE_BARREL(9, 2));
    AppendSprite(ctx, SPRITE_LAMP(5, 10));
    AppendSprite(ctx, SPRITE_COLUMN(4, 4));
    AppendEntity(ctx, ENTITY_GOBLIN(2, 2));
    AppendEntity(ctx, ENTITY_SKELETON(10, 10));
    AppendEntity(ctx, ENTITY_EYE(14, 2));
    AppendEntity(ctx, ENTITY_MUSHROOM(12, 12));
    
    int** map = ExportSearchMap(ctx);
    for (size_t i = 0; i < ctx->entity_count; i++) {
        if(ctx->entities[i]->default_path.length == 0) {
            ctx->entities[i]->default_path = 
                GenerateRandomLoopPath(map, ctx->map_width, ctx->map_height, 12, ctx->entities[i]->sprite->x, ctx->entities[i]->sprite->y);
        }
    }
    MapFree(map, ctx->map_height);
}

void Level3(Context* ctx)
{
    LoadLevelMap(ctx, "levels/3.lvl");
    
}

void Level4(Context* ctx)
{
    LoadLevelMap(ctx, "levels/4.lvl");
    
}

