#include "core.h"
#include <assert.h>


void castor_AppendEntity(castor_Context* ctx, castor_Entity* entity)
{
    if (ctx->level.entity_count >= MAX_ENTITIES) return;

    castor_AppendSprite(ctx, entity->sprite);

    entity->index = ctx->level.entity_count;
    ctx->level.entities[ctx->level.entity_count++] = entity;
}

void castor_UpdateEntities(castor_Context* ctx, float deltaTime)
{
    castor_Map* map = castor_ExportSearchMap(ctx);
    
    for (size_t i = 0; i < ctx->level.entity_count; i++) {
        castor_Entity* e = ctx->level.entities[i];
        if(!e) continue;
        if(e->health <= 0) {
            // TODO: Proper death method
            castor_RemoveEntity(ctx, e);
            continue;
        }

        if (e->move) {
            e->move(e, (const int**) map->grid, ctx->level.map->w, ctx->level.map->h, ctx->level.player, deltaTime);
        }
    }

    castor_MapFree(&map);
}

void castor_RemoveEntity(castor_Context* ctx, const castor_Entity* entity)
{
    assert(ctx);
    assert(entity);

    size_t index = entity->index;
    if (index >= ctx->level.entity_count) {
        fprintf(stderr, "Error: Invalid entity index!\n");
        return;
    }

    // Remove associated sprite if available
    if (entity->sprite) {
        castor_RemoveSprite(ctx, entity->sprite);
    }
    castor_EntityFree(&ctx->level.entities[index]);

    // Shift entities left
    for (size_t i = index; i < ctx->level.entity_count - 1; i++) {
        ctx->level.entities[i] = ctx->level.entities[i + 1];
        ctx->level.entities[i]->index = i;
    }

    ctx->level.entities[ctx->level.entity_count - 1] = NULL;
    ctx->level.entity_count--;
}

void castor_FreeEntities(castor_Context* ctx)
{
    for(size_t i = 0; i < ctx->level.entity_count; i++){
        if(ctx->level.entities[i]){
            castor_EntityFree(&ctx->level.entities[i]);
        }
    }
    ctx->level.entity_count = 0;
}

