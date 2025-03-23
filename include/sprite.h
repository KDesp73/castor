#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include <stddef.h>

#define SPRITE_UP 0.5
#define SPRITE_MIDDLE 0
#define SPRITE_DOWN -0.5

#define SPRITE_PLACE(_x, _y) \
    .x = (_x) + 0.5, \
    .y = (_y) + 0.5

typedef struct {
    float x, y, z;
    int width, height;
    float distance;
    size_t texture_id;
    float scale;
    bool collision;
    bool is_animation;
    size_t index;
    char id[16];
} Sprite;

Sprite* SpriteNew(float x, float y, float z, size_t textureId, float scale, bool collision, const char* id);
void SpriteFree(Sprite** s);

int SpriteCmp(const void* a, const void* b);


#endif // SPRITE_H
