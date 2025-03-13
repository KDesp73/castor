#include "sprite.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int SpriteCmp(const void* a, const void* b)
{
    const Sprite* spriteA = *(const Sprite**)a;
    const Sprite* spriteB = *(const Sprite**)b;

    if (spriteA->distance < spriteB->distance) return 1; // Farther comes first
    if (spriteA->distance > spriteB->distance) return -1; // Closer comes last
    return 0;
}

Sprite* SpriteNew(float x, float y, float z, size_t textureId, float scale, bool collision)
{
    assert(textureId >= 0);

    Sprite* s = malloc(sizeof(Sprite));
    if(!s) {
        fprintf(stderr, "Could not allocate memory for sprite\n");
        return NULL;
    }

    s->x = x;
    s->y = y;
    s->z = z;
    s->texture_id = textureId;
    s->scale = scale;
    s->collision = collision;

    return s;
}

void SpriteFree(Sprite** s){
    if(*s){
        free(*s);
        *s = NULL;
    }
}

