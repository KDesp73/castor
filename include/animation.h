#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL_render.h>
typedef struct {
    SDL_Texture *texture;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    size_t currentFrame;
    Uint32 lastUpdateTime;
    Uint32 frameDelay;
} Animation;

Animation LoadAnimation(SDL_Renderer *renderer, const char *filePath, int frameWidth, int frameHeight, Uint32 rate);
void RenderAnimation(SDL_Renderer *renderer, Animation *anim, float scale, int x, int y, int frameIndex);
void UpdateAnimation(Animation *anim, Uint32 currentTime);
void FreeAnimation(Animation *anim);

#endif // ANIMATION_H
