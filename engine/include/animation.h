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
} castor_Animation;

castor_Animation castor_LoadAnimation(SDL_Renderer *renderer, const char *filePath, int frameWidth, int frameHeight, Uint32 rate);
void castor_RenderAnimation(SDL_Renderer *renderer, castor_Animation *anim, float scale, int x, int y, int frameIndex);
void castor_UpdateAnimation(castor_Animation *anim, Uint32 currentTime);
void castor_FreeAnimation(castor_Animation *anim);

#endif // ANIMATION_H
