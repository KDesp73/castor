#include "context.h"
#include "map.h"
#include <assert.h>


void AppendEntity(Context* ctx, Entity* entity)
{
    if (ctx->level.entity_count >= MAX_ENTITIES) return;

    AppendSprite(ctx, entity->sprite);

    entity->index = ctx->level.entity_count;
    ctx->level.entities[ctx->level.entity_count++] = entity;
    printf("Appended entity '%s' at index: %zu\n", entity->id, entity->index);
}

void UpdateEntities(Context* ctx, float deltaTime)
{
    int** map = ExportSearchMap(ctx);
    
    for (size_t i = 0; i < ctx->level.entity_count; i++) {
        Entity* e = ctx->level.entities[i];
        if(!e) continue;
        if(e->health <= 0) {
            // TODO: Proper death method
            RemoveEntity(ctx, e);
            continue;
        }

        if (e->move) {
            e->move(e, (const int**)map, ctx->level.map_width, ctx->level.map_height, ctx->level.player, deltaTime);
        }
    }

    MapFree(map, ctx->level.map_height);
}

void RemoveEntity(Context* ctx, const Entity* entity)
{
    assert(ctx);
    assert(entity);

    size_t index = entity->index;
    printf("Removing entity '%s' from index: %zu\n", entity->id, index);
    if (index >= ctx->level.entity_count) {
        fprintf(stderr, "Error: Invalid entity index!\n");
        return;
    }

    // Remove associated sprite if available
    if (entity->sprite) {
        RemoveSprite(ctx, entity->sprite);
    }
    EntityFree(&ctx->level.entities[index]);

    // Shift entities left
    for (size_t i = index; i < ctx->level.entity_count - 1; i++) {
        ctx->level.entities[i] = ctx->level.entities[i + 1];
        ctx->level.entities[i]->index = i;
    }

    ctx->level.entities[ctx->level.entity_count - 1] = NULL;
    ctx->level.entity_count--;
}

void FreeEntities(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.entity_count; i++){
        if(ctx->level.entities[i]){
            EntityFree(&ctx->level.entities[i]);
        }
    }
    ctx->level.entity_count = 0;
}

