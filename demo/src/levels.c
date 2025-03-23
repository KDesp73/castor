#include "levels.h"
#include "context.h"
#include "entity.h"
#include "event.h"
#include "events.h"
#include "game_player.h"
#include "inventory.h"
#include "map.h"
#include "player.h"
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

#define LEVEL_HEADER(path) \
    do { \
        InventoryClear(); \
        LoadLevelMap(ctx, path); \
        if(PLR.health <= 0) \
            PLR.health = PLR.maxHealth; \
    } while(0)

#define LEVEL_FOOTER() \
    do { \
        GivePaths(ctx); \
    } while(0)

#define LEVEL_COMMON_EVENTS() \
    do { \
        AppendEvent(ctx, EventNew(ctx, false, 200, PickItemTrigger, PickItemAction)); \
        AppendEvent(ctx, EventNew(ctx, false, 300, PlayerAttackTrigger, PlayerAttackAction)); \
        AppendEvent(ctx, EventNew(ctx, false, 1500, EnemyAttackTrigger, EnemyAttackAction)); \
    } while(0)

void Level0(Context* ctx)
{
    LEVEL_HEADER("levels/0.lvl");

    PlayerPlace(ctx->level.player, 4, 6, 65);

    AppendItem(ctx, ITEM_KEY(10, 10));
    AppendItem(ctx, ITEM_SWORD(5, 10));
    AppendItem(ctx, ITEM_GLASSES(10, 5));

    AppendEvent(ctx, EventNew(ctx, false, 300, Door0Trigger, DoorTooltipAction));
    AppendEvent(ctx, EventNew(ctx, false, 100, Door0Trigger, DoorKeyAction));

    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level1(Context* ctx)
{
    LEVEL_HEADER("levels/1.lvl");

    PlayerPlace(ctx->level.player, 4, 6, 65);
    
    AppendSprite(ctx, SPRITE_COLUMN(9, 9));
    AppendEntity(ctx, ENTITY_GOBLIN(3, 5));
    AppendItem(ctx, ITEM_KEY(12, 3));
    AppendItem(ctx, ITEM_SWORD(2, 8));

    AppendEvent(ctx, EventNew(ctx, false, 300, Door1Trigger, DoorTooltipAction));
    AppendEvent(ctx, EventNew(ctx, false, 100, Door1Trigger, DoorKeyAction));
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level2(Context* ctx)
{
    LEVEL_HEADER("levels/2.lvl");

    PlayerPlace(ctx->level.player, 4, 6, 65);

    AppendSprite(ctx, SPRITE_BARREL(9, 2));
    AppendSprite(ctx, SPRITE_LAMP(5, 10));
    AppendSprite(ctx, SPRITE_COLUMN(4, 4));

    AppendEntity(ctx, ENTITY_EYE(14, 3));

    AppendItem(ctx, ITEM_KEY(10, 10));
    AppendItem(ctx, ITEM_GLASSES(13, 10));

    AppendEvent(ctx, EventNew(ctx, false, 300, Door2Trigger, DoorTooltipAction));
    AppendEvent(ctx, EventNew(ctx, false, 100, Door2Trigger, DoorKeyAction));
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level3(Context* ctx)
{
    LEVEL_HEADER("levels/3.lvl");

    PlayerPlace(ctx->level.player, 4, 6, 65);

    AppendItem(ctx, ITEM_KEY(5, 13));

    AppendEvent(ctx, EventNew(ctx, false, 300, Door3Trigger, DoorTooltipAction));
    AppendEvent(ctx, EventNew(ctx, false, 100, Door3Trigger, DoorKeyAction));
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level4(Context* ctx)
{
    LEVEL_HEADER("levels/4.lvl");

    PlayerPlace(ctx->level.player, 4, 6, 65);

    AppendEntity(ctx, ENTITY_SKELETON(1, 2));
    AppendEntity(ctx, ENTITY_SKELETON(15, 19));
    AppendEntity(ctx, ENTITY_MUSHROOM(8, 23));

    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

