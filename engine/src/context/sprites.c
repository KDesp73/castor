#include "context.h"
#include <assert.h>


void castor_AppendSprite(castor_Context* ctx, castor_Sprite* sprite)
{
    if (ctx->level.sprite_count >= MAX_SPRITES) return;

    sprite->index = ctx->level.sprite_count;
    ctx->level.sprites[ctx->level.sprite_count++] = sprite;
}

void castor_RemoveSprite(castor_Context* ctx, const castor_Sprite* sprite)
{
    assert(ctx);
    assert(sprite);

    size_t index = sprite->index;
    if (index >= ctx->level.sprite_count) {
        fprintf(stderr, "Error: Invalid sprite index!\n");
        return;
    }

    castor_SpriteFree(&ctx->level.sprites[index]);

    // Shift remaining sprites left
    for (size_t i = index; i < ctx->level.sprite_count - 1; i++) {
        ctx->level.sprites[i] = ctx->level.sprites[i + 1];
        ctx->level.sprites[i]->index = i;
    }

    ctx->level.sprites[ctx->level.sprite_count - 1] = NULL;
    ctx->level.sprite_count--;
}


void castor_FreeSprites(castor_Context* ctx)
{
    for(size_t i = 0; i < ctx->level.sprite_count; i++){
        if(ctx->level.sprites[i]){
            castor_SpriteFree(&ctx->level.sprites[i]);
        }
    }
    ctx->level.sprite_count = 0;
}

