#include "world.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int castor_SpriteCmp(const void* a, const void* b)
{
    const castor_Sprite* spriteA = *(const castor_Sprite**)a;
    const castor_Sprite* spriteB = *(const castor_Sprite**)b;

    if(!spriteA || !spriteB) return 0;

    if (spriteA->distance < spriteB->distance) return 1; // Farther comes first
    if (spriteA->distance > spriteB->distance) return -1; // Closer comes last
    return 0;
}

castor_Sprite* castor_SpriteNew(float x, float y, float z, size_t textureId, float scale, bool collision, const char* id)
{
    assert(textureId >= 0);

    castor_Sprite* s = malloc(sizeof(castor_Sprite));
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
    s->index = -1;
    strncpy(s->id, id, 15);

    return s;
}

void castor_SpriteFree(castor_Sprite** s){
    if(*s){
        free(*s);
        *s = NULL;
    }
}

