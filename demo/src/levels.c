#include "levels.h"
#include "context.h"
#include "entity.h"
#include "event.h"
#include "events.h"
#include "map.h"
#include "sprite.h"
#include "objects.h"

static void GivePaths(Context* ctx)
{
    int** map = ExportSearchMap(ctx);
    for (size_t i = 0; i < ctx->level.entity_count; i++) {
        if(ctx->level.entities[i]->default_path.length == 0) {
            ctx->level.entities[i]->default_path = 
                GenerateRandomLoopPath(map, ctx->level.map_width, ctx->level.map_height, 12, ctx->level.entities[i]->sprite->x, ctx->level.entities[i]->sprite->y);
        }
    }
    MapFree(map, ctx->level.map_height);

}

void Level0(Context* ctx)
{
    LoadLevelMap(ctx, "levels/0.lvl");

    AppendItem(ctx, ITEM_KEY(10, 10));

    AppendEvent(ctx, EventNew(ctx, false, 200, PickItemTrigger, PickItemAction));
    AppendEvent(ctx, EventNew(ctx, false, 300, Door0Trigger, DoorTooltipAction));
    AppendEvent(ctx, EventNew(ctx, false, 100, Door0Trigger, DoorKeyAction));
}

void Level1(Context* ctx)
{
    LoadLevelMap(ctx, "levels/1.lvl");
    
    AppendSprite(ctx, SPRITE_COLUMN(9, 9));
}

void Level2(Context* ctx)
{
    LoadLevelMap(ctx, "levels/2.lvl");

    AppendSprite(ctx, SPRITE_BARREL(9, 2));
    AppendSprite(ctx, SPRITE_LAMP(5, 10));
    AppendSprite(ctx, SPRITE_COLUMN(4, 4));

    // AppendEntity(ctx, ENTITY_EYE(14, 3));

    AppendItem(ctx, ITEM_KEY(10, 10));
    AppendItem(ctx, ITEM_GLASSES(13, 10));

    AppendEvent(ctx, EventNew(ctx, false, 200, PickItemTrigger, PickItemAction));
    AppendEvent(ctx, EventNew(ctx, false, 300, PlayerAttackTrigger, PlayerAttackAction));
    AppendEvent(ctx, EventNew(ctx, false, 300, Door2Trigger, DoorTooltipAction));
    AppendEvent(ctx, EventNew(ctx, false, 100, Door2Trigger, DoorKeyAction));

    
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

