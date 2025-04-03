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

static void GivePaths(castor_Context* ctx)
{
    int** map = castor_ExportSearchMap(ctx);
    for (size_t i = 0; i < ctx->level.entity_count; i++) {
        if(ctx->level.entities[i]->default_path.length == 0) {
            ctx->level.entities[i]->default_path = 
                castor_GenerateRandomLoopPath(map, ctx->level.map_width, ctx->level.map_height, 12, ctx->level.entities[i]->sprite->x, ctx->level.entities[i]->sprite->y);
        }
    }
    castor_MapFree(map, ctx->level.map_height);

}

#define LEVEL_HEADER(path) \
    do { \
        InventoryClear(); \
        castor_LoadLevelMap(ctx, path); \
        if(PLR.health <= 0) \
            PLR.health = PLR.maxHealth; \
    } while(0)

#define LEVEL_FOOTER() \
    do { \
        GivePaths(ctx); \
    } while(0)

#define LEVEL_COMMON_EVENTS() \
    do { \
        castor_AppendEvent(ctx, castor_EventNew(ctx, false, 200, PickItemTrigger, PickItemAction)); \
        castor_AppendEvent(ctx, castor_EventNew(ctx, false, 300, PlayerAttackTrigger, PlayerAttackAction)); \
        castor_AppendEvent(ctx, castor_EventNew(ctx, false, 2000, EnemyAttackTrigger, EnemyAttackAction)); \
    } while(0)

#define LEVEL_DOOR_EVENTS(index) \
    do { \
        castor_AppendEvent(ctx, castor_EventNew(ctx, false, 300, Door##index##Trigger, DoorTooltipAction)); \
        castor_AppendEvent(ctx, castor_EventNew(ctx, false, 50, Door##index##Trigger, DoorKeyAction)); \
    } while(0)

#define LEVEL_GLITCH_EVENT(index) \
    castor_Event* glitch = castor_EventNew(ctx, false, 20000, GlitchTrigger, Glitch##index##Action); \
    strcpy(glitch->id, "glitch"); \
    castor_AppendEvent(ctx, glitch);


void Level0(castor_Context* ctx)
{
    LEVEL_HEADER("levels/0.lvl");

    castor_PlayerPlace(ctx->level.player, 2, 2, 0);

    castor_AppendSprite(ctx, SPRITE_LAMP(7, 8));
    castor_AppendSprite(ctx, SPRITE_BARREL(3, 6));
    castor_AppendSprite(ctx, SPRITE_BARREL(3, 7));
    castor_AppendSprite(ctx, SPRITE_BARREL(4, 6));
    castor_AppendSprite(ctx, SPRITE_COLUMN(13, 14));

    castor_AppendItem(ctx, ITEM_KEY(12, 2));
    castor_AppendItem(ctx, ITEM_GLASSES(4, 14));

    LEVEL_DOOR_EVENTS(0);
    LEVEL_GLITCH_EVENT(0);
    LEVEL_COMMON_EVENTS();
    castor_AppendEvent(ctx, castor_EventNew(ctx, false, 2000, GlassesTipTrigger, GlassesTipAction));


    LEVEL_FOOTER();
}

void Level1(castor_Context* ctx)
{
    LEVEL_HEADER("levels/1.lvl");

    castor_PlayerPlace(ctx->level.player, 12, 12, 90);

    castor_AppendSprite(ctx, SPRITE_LAMP(11.5, 11.5));
    castor_AppendSprite(ctx, SPRITE_BARREL(5, 20));

    castor_AppendItem(ctx, ITEM_SWORD(9, 12));
    castor_AppendItem(ctx, ITEM_GLASSES(18, 2));
    castor_AppendItem(ctx, ITEM_KEY(1, 16));
    
    // castor_AppendEntity(ctx, ENTITY_GOBLIN(5, 17));
    castor_AppendEntity(ctx, ENTITY_MUSHROOM(20, 19));

    LEVEL_DOOR_EVENTS(1);
    LEVEL_GLITCH_EVENT(1);
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level2(castor_Context* ctx)
{
    LEVEL_HEADER("levels/2.lvl");

    castor_PlayerPlace(ctx->level.player, 2, 2, 0);

    castor_AppendSprite(ctx, SPRITE_BARREL(9, 2));
    castor_AppendSprite(ctx, SPRITE_LAMP(5, 10));
    castor_AppendSprite(ctx, SPRITE_COLUMN(4, 4));
    castor_AppendSprite(ctx, SPRITE_BARREL(5, 14));

    castor_AppendEntity(ctx, ENTITY_EYE(14, 3));
    castor_AppendEntity(ctx, ENTITY_MUSHROOM(12, 12));

    castor_AppendItem(ctx, ITEM_KEY(4, 14));
    castor_AppendItem(ctx, ITEM_GLASSES(14, 2));
    castor_AppendItem(ctx, ITEM_SWORD(11, 2));

    LEVEL_DOOR_EVENTS(2);
    LEVEL_GLITCH_EVENT(2);
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level3(castor_Context* ctx)
{
    LEVEL_HEADER("levels/3.lvl");

    castor_PlayerPlace(ctx->level.player, 1.5, 1.5, 0);

    castor_AppendItem(ctx, ITEM_KEY(11, 11));

    LEVEL_DOOR_EVENTS(3);
    LEVEL_COMMON_EVENTS();

    LEVEL_FOOTER();
}

void Level4(castor_Context* ctx)
{
    LEVEL_HEADER("levels/4.lvl");

    castor_PlayerPlace(ctx->level.player, 4, 3, 0);

    castor_AppendSprite(ctx, SPRITE_BARREL(6, 4));
    castor_AppendSprite(ctx, SPRITE_BARREL(19, 6));
    castor_AppendSprite(ctx, SPRITE_BARREL(23, 18));
    castor_AppendSprite(ctx, SPRITE_BARREL(24, 17));
    castor_AppendSprite(ctx, SPRITE_BARREL(23, 17));
    castor_AppendSprite(ctx, SPRITE_COLUMN(3, 13));
    castor_AppendSprite(ctx, SPRITE_LAMP(27, 1));
    castor_AppendSprite(ctx, SPRITE_LAMP(14, 22));

    castor_AppendItem(ctx, ITEM_GLASSES(5, 13));
    castor_AppendItem(ctx, ITEM_SWORD(7, 2));
    castor_AppendItem(ctx, ITEM_KEY(1, 30));

    castor_AppendEntity(ctx, ENTITY_GOBLIN(18, 11));
    castor_AppendEntity(ctx, ENTITY_EYE(11, 21));
    castor_AppendEntity(ctx, ENTITY_EYE(4, 6));
    castor_AppendEntity(ctx, ENTITY_SKELETON(28, 8));
    castor_AppendEntity(ctx, ENTITY_SKELETON(17, 3));
    castor_AppendEntity(ctx, ENTITY_MUSHROOM(11, 3));

    LEVEL_DOOR_EVENTS(4);
    LEVEL_GLITCH_EVENT(4);
    LEVEL_COMMON_EVENTS();
    castor_AppendEvent(ctx, castor_EventNew(ctx, false, 100, TeleportTrigger, TeleportAction));

    LEVEL_FOOTER();
}

void Level5(castor_Context* ctx)
{
    LEVEL_HEADER("levels/credits.lvl");

    castor_PlayerPlace(ctx->level.player, 12, 12, 0);

    castor_AppendSprite(ctx, SPRITE_COLUMN(7, 7));
    castor_AppendSprite(ctx, SPRITE_COLUMN(7, 16));
    castor_AppendSprite(ctx, SPRITE_COLUMN(16, 7));
    castor_AppendSprite(ctx, SPRITE_COLUMN(16, 16));

    castor_AppendItem(ctx, ITEM_SWORD(21, 11));

    castor_AppendEntity(ctx, ENTITY_GRIMM(2, 9));

    LEVEL_GLITCH_EVENT(5);
    LEVEL_COMMON_EVENTS();
    
    castor_AppendEvent(ctx, castor_EventNew(ctx, true, 100, CreditsTrigger, CreditsAction));
    castor_AppendEvent(ctx, castor_EventNew(ctx, true, 100, EETrigger, EEAction));


    LEVEL_FOOTER();
}
