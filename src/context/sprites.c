#include "context.h"
#include <assert.h>


void AppendSprite(Context* ctx, Sprite* sprite)
{
    if (ctx->level.sprite_count >= MAX_SPRITES) return;

    sprite->index = ctx->level.sprite_count;
    ctx->level.sprites[ctx->level.sprite_count++] = sprite;
    printf("Appended sprite at index: %zu\n", sprite->index);
}

void PrintSprites(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.sprite_count; i++) {
        Sprite* s = ctx->level.sprites[i];
        printf("%s\n", s->id);
    }
}

void RemoveSprite(Context* ctx, const Sprite* sprite)
{
    assert(ctx);
    assert(sprite);
    PrintSprites(ctx);

    size_t index = sprite->index;
    printf("Removing sprite from index: %zu\n", index);
    if (index >= ctx->level.sprite_count) {
        fprintf(stderr, "Error: Invalid sprite index!\n");
        return;
    }

    SpriteFree(&ctx->level.sprites[index]);

    // Shift remaining sprites left
    for (size_t i = index; i < ctx->level.sprite_count - 1; i++) {
        ctx->level.sprites[i] = ctx->level.sprites[i + 1];
        ctx->level.sprites[i]->index = i;
        printf("Sprite %s moved to %zu\n", ctx->level.sprites[i]->id, i);
    }

    ctx->level.sprites[ctx->level.sprite_count - 1] = NULL;
    ctx->level.sprite_count--;
    PrintSprites(ctx);
}


void FreeSprites(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.sprite_count; i++){
        if(ctx->level.sprites[i]){
            SpriteFree(&ctx->level.sprites[i]);
        }
    }
    ctx->level.sprite_count = 0;
}

