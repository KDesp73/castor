#include "levels.h"
#include "context.h"
#include "entity.h"
#include "event.h"
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
bool trigger1(Event* evt)
{
    Context* ctx = evt->ctx;
    Player* player = ctx->player;

    return (
        (player->X >= 4 && player->X < 5) &&
        (player->Y >= 8 && player->Y < 9)
    );
}

void action1(Event* evt)
{
    Context* ctx = evt->ctx;

    UIToast toast = {0};
    UIToastInit(&toast, "Entered (4, 8) square", 2000);
    UIAppendToast(&ctx->ui, toast);
}

void Level2(Context* ctx)
{
    LoadLevelMap(ctx, "levels/2.lvl");

    AppendSprite(ctx, SPRITE_BARREL(9, 2));
    AppendSprite(ctx, SPRITE_LAMP(5, 10));
    AppendSprite(ctx, SPRITE_COLUMN(4, 4));
    // AppendEntity(ctx, ENTITY_GOBLIN(2, 2));
    // AppendEntity(ctx, ENTITY_SKELETON(10, 10));
    // AppendEntity(ctx, ENTITY_EYE(14, 2));
    AppendEntity(ctx, ENTITY_MUSHROOM(12, 12));

    Event* event = EventNew(ctx, false, trigger1, action1);
    AppendEvent(ctx, event);
    
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

