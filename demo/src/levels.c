#include "levels.h"
#include "context.h"
#include "entity.h"
#include "event.h"
#include "events.h"
#include "map.h"
#include "sprite.h"
#include "objects.h"
#include "ui.h"

static void GivePaths(Context* ctx)
{
    int** map = ExportSearchMap(ctx);
    for (size_t i = 0; i < ctx->entity_count; i++) {
        if(ctx->entities[i]->default_path.length == 0) {
            ctx->entities[i]->default_path = 
                GenerateRandomLoopPath(map, ctx->map_width, ctx->map_height, 12, ctx->entities[i]->sprite->x, ctx->entities[i]->sprite->y);
        }
    }
    MapFree(map, ctx->map_height);

}

void Level1(Context* ctx)
{
    LoadLevelMap(ctx, "levels/1.lvl");
    
    AppendSprite(ctx, SPRITE_COLUMN(9, 9));
}

// Trigger once the player enters the (4,8) square

void Level2(Context* ctx)
{
    LoadLevelMap(ctx, "levels/2.lvl");

    AppendSprite(ctx, SPRITE_BARREL(9, 2));
    AppendSprite(ctx, SPRITE_LAMP(5, 10));
    AppendSprite(ctx, SPRITE_COLUMN(4, 4));

    AppendEntity(ctx, ENTITY_EYE(14, 3));

    AppendItem(ctx, ITEM_KEY(10, 10));

    AppendEvent(ctx, EventNew(ctx, true, 1000, trigger1, action1));
    AppendEvent(ctx, EventNew(ctx, false, 200, PickItemTrigger, PickItemAction));
    AppendEvent(ctx, EventNew(ctx, false, 300, PlayerAttackTrigger, PlayerAttackAction));
    
    GivePaths(ctx);
}

void Level3(Context* ctx)
{
    LoadLevelMap(ctx, "levels/3.lvl");
    
}

void Level4(Context* ctx)
{
    LoadLevelMap(ctx, "levels/4.lvl");
    
}

