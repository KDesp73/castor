#include "context.h"
#include <assert.h>

static float PseudoRandomValue(uintptr_t seed)
{
    seed = (seed ^ 0xDEADBEEF) + (seed << 4);
    seed = seed ^ (seed >> 7);
    return (float)(seed % 1000) / 1000.0f;
}

void castor_ItemsIdle(castor_Context* ctx, float deltaTime)
{
    float baseAmplitude = 1.0f;
    float baseSpeed = 2.0f;

    for (size_t i = 0; i < ctx->level.item_count; i++) {
        castor_Item* item = ctx->level.items[i];
        if (item && item->sprite) {
            uintptr_t seed = (uintptr_t)item;
            float amplitude = baseAmplitude * (0.75f + 0.5f * PseudoRandomValue(seed));
            float speed = baseSpeed * (0.75f + 0.5f * PseudoRandomValue(seed + 1234));
            float phase = 2.0f * M_PI * PseudoRandomValue(seed + 5678);

            float offset = sinf(deltaTime * speed + phase) * amplitude;
            item->sprite->z = item->baseZ + offset;
        }
    }
}


void castor_AppendItem(castor_Context* ctx, castor_Item* item)
{
    if (ctx->level.item_count >= MAX_ITEMS) return;

    castor_AppendSprite(ctx, item->sprite);

    item->index = ctx->level.item_count;
    ctx->level.items[ctx->level.item_count++] = item;
}

void castor_RemoveItem(castor_Context* ctx, const castor_Item* item)
{
    assert(ctx);
    assert(item);

    size_t index = item->index;
    if (index >= ctx->level.item_count) {
        fprintf(stderr, "Error: Invalid item index!\n");
        return;
    }

    if (item->sprite) {
        castor_RemoveSprite(ctx, item->sprite);
    }
    castor_ItemFree(&ctx->level.items[index]);

    for (size_t i = index; i < ctx->level.item_count - 1; i++) {
        ctx->level.items[i] = ctx->level.items[i + 1];

        if(ctx->level.items[i])
            ctx->level.items[i]->index = i;
    }

    ctx->level.items[ctx->level.item_count - 1] = NULL;
    ctx->level.item_count--;
}

void castor_FreeItems(castor_Context* ctx)
{
    for(size_t i = 0; i < ctx->level.item_count; i++){
        if(ctx->level.items[i]){
            castor_ItemFree(&ctx->level.items[i]);
        }
    }
    ctx->level.item_count = 0;
}
