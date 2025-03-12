#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include <stddef.h>
typedef struct {
    float x, y, z;
    int width, height;
    float distance;
    size_t texture_id;
    float scale;
    bool collision;
} Sprite;

static inline int SpriteCmp(const void* a, const void* b)
{
    const Sprite* spriteA = (const Sprite*)a;
    const Sprite* spriteB = (const Sprite*)b;

    // Sorting in descending order based on distance
    if (spriteA->distance < spriteB->distance) return 1;
    if (spriteA->distance > spriteB->distance) return -1;
    return 0;
}


#endif // SPRITE_H
