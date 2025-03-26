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
#include <string.h>

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
        AppendEvent(ctx, EventNew(ctx, false, 2000, EnemyAttackTrigger, EnemyAttackAction)); \
    } while(0)

#define LEVEL_DOOR_EVENTS(index) \
    do { \
        AppendEvent(ctx, EventNew(ctx, false, 300, Door##index##Trigger, DoorTooltipAction)); \
        AppendEvent(ctx, EventNew(ctx, false, 50, Door##index##Trigger, DoorKeyAction)); \
    } while(0)

#define LEVEL_GLITCH_EVENT(index) \
    Event* glitch = EventNew(ctx, false, 30000, GlitchTrigger, Glitch##index##Action); \
    strcpy(glitch->id, "glitch"); \
    AppendEvent(ctx, glitch);


void Level0(Context* ctx)
{
    LEVEL_HEADER("levels/0.lvl");

    PlayerPlace(ctx->level.player, 2, 2, 0);

    AppendSprite(ctx, SPRITE_LAMP(7, 8));
    AppendSprite(ctx, SPRITE_BARREL(3, 6));
    AppendSprite(ctx, SPRITE_BARREL(3, 7));
    AppendSprite(ctx, SPRITE_BARREL(4, 6));
    AppendSprite(ctx, SPRITE_COLUMN(13, 14));

    AppendItem(ctx, ITEM_KEY(12, 2));
    AppendItem(ctx, ITEM_GLASSES(4, 14));

    LEVEL_DOOR_EVENTS(0);
    LEVEL_GLITCH_EVENT(0);
    LEVEL_COMMON_EVENTS();
    AppendEvent(ctx, EventNew(ctx, false, 2000, GlassesTipTrigger, GlassesTipAction));


    LEVEL_FOOTER();
}

void Level1(Context* ctx)
{
    LEVEL_HEADER("levels/1.lvl");

    PlayerPlace(ctx->level.player, 12, 12, 90);

    AppendSprite(ctx, SPRITE_LAMP(11.5, 11.5));
    AppendSprite(ctx, SPRITE_BARREL(5, 20));

    AppendItem(ctx, ITEM_SWORD(9, 12));
    AppendItem(ctx, ITEM_GLASSES(18, 2));
    AppendItem(ctx, ITEM_KEY(1, 16));
    
    // AppendEntity(ctx, ENTITY_GOBLIN(5, 17));
    AppendEntity(ctx, ENTITY_MUSHROOM(20, 19));

    LEVEL_DOOR_EVENTS(1);
    LEVEL_GLITCH_EVENT(1);
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level2(Context* ctx)
{
    LEVEL_HEADER("levels/2.lvl");

    PlayerPlace(ctx->level.player, 2, 2, 0);

    AppendSprite(ctx, SPRITE_BARREL(9, 2));
    AppendSprite(ctx, SPRITE_LAMP(5, 10));
    AppendSprite(ctx, SPRITE_COLUMN(4, 4));
    AppendSprite(ctx, SPRITE_BARREL(5, 14));

    AppendEntity(ctx, ENTITY_EYE(14, 3));

    AppendItem(ctx, ITEM_KEY(4, 14));
    AppendItem(ctx, ITEM_GLASSES(14, 2));
    AppendItem(ctx, ITEM_SWORD(11, 2));

    LEVEL_DOOR_EVENTS(2);
    LEVEL_GLITCH_EVENT(2);
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level3(Context* ctx)
{
    LEVEL_HEADER("levels/3.lvl");

    PlayerPlace(ctx->level.player, 1.5, 1.5, 0);

    AppendItem(ctx, ITEM_KEY(11, 11));

    LEVEL_DOOR_EVENTS(3);
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level4(Context* ctx)
{
    LEVEL_HEADER("levels/4.lvl");

    PlayerPlace(ctx->level.player, 4, 3, 0);

    AppendSprite(ctx, SPRITE_BARREL(6, 4));
    AppendSprite(ctx, SPRITE_BARREL(19, 6));
    AppendSprite(ctx, SPRITE_BARREL(23, 18));
    AppendSprite(ctx, SPRITE_BARREL(24, 17));
    AppendSprite(ctx, SPRITE_BARREL(23, 17));
    AppendSprite(ctx, SPRITE_COLUMN(16, 3));
    AppendSprite(ctx, SPRITE_COLUMN(3, 13));
    AppendSprite(ctx, SPRITE_LAMP(27, 1));
    AppendSprite(ctx, SPRITE_LAMP(14, 22));

    AppendItem(ctx, ITEM_GLASSES(5, 13));
    AppendItem(ctx, ITEM_SWORD(7, 2));
    AppendItem(ctx, ITEM_KEY(1, 30));

    AppendEntity(ctx, ENTITY_GOBLIN(18, 11));
    AppendEntity(ctx, ENTITY_EYE(11, 21));
    AppendEntity(ctx, ENTITY_EYE(4, 6));
    AppendEntity(ctx, ENTITY_SKELETON(28, 8));
    AppendEntity(ctx, ENTITY_SKELETON(17, 3));
    AppendEntity(ctx, ENTITY_MUSHROOM(11, 3));

    LEVEL_DOOR_EVENTS(4);
    LEVEL_GLITCH_EVENT(4);
    LEVEL_COMMON_EVENTS();
    AppendEvent(ctx, EventNew(ctx, false, 100, TeleportTrigger, TeleportAction));

    LEVEL_FOOTER();
}

void Level5(Context* ctx)
{
    LEVEL_HEADER("levels/credits.lvl");

    PlayerPlace(ctx->level.player, 12, 12, 0);

    AppendSprite(ctx, SPRITE_COLUMN(7, 7));
    AppendSprite(ctx, SPRITE_COLUMN(7, 16));
    AppendSprite(ctx, SPRITE_COLUMN(16, 7));
    AppendSprite(ctx, SPRITE_COLUMN(16, 16));

    AppendItem(ctx, ITEM_SWORD(21, 11));

    AppendEntity(ctx, ENTITY_GRIMM(2, 9));

    LEVEL_GLITCH_EVENT(5);
    LEVEL_COMMON_EVENTS();
    
    AppendEvent(ctx, EventNew(ctx, true, 100, CreditsTrigger, CreditsAction));


    LEVEL_FOOTER();
}
