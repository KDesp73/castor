#include "asset.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>

castor_Animation castor_LoadAnimation(SDL_Renderer *renderer, const char *filePath, int frameWidth, int frameHeight, Uint32 rate)
{
    castor_Animation anim = {.frameDelay = rate};

    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return anim;
    }

    anim.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!anim.texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return anim;
    }

    // Get the full image width and height
    int imgWidth, imgHeight;
    SDL_QueryTexture(anim.texture, NULL, NULL, &imgWidth, &imgHeight);

    anim.frameWidth = frameWidth;
    anim.frameHeight = frameHeight;
    anim.totalFrames = imgWidth / frameWidth; // Assuming frames are laid out in a row

    return anim;
}

void castor_UpdateAnimation(castor_Animation *anim, Uint32 currentTime)
{
    if (currentTime > anim->lastUpdateTime + anim->frameDelay) {
        anim->currentFrame = (anim->currentFrame + 1) % anim->totalFrames;
        anim->lastUpdateTime = currentTime;
    }
}

void castor_RenderAnimation(SDL_Renderer *renderer, castor_Animation *anim, float scale, int x, int y, int frameIndex)
{
    if (!anim || !anim->texture || frameIndex >= anim->totalFrames) return;

    SDL_Rect srcRect = { frameIndex * anim->frameWidth, 0, anim->frameWidth, anim->frameHeight };
    SDL_Rect dstRect = { x, y, anim->frameWidth * scale, anim->frameHeight* scale};

    SDL_RenderCopy(renderer, anim->texture, &srcRect, &dstRect);
    anim->currentFrame = frameIndex;
}

void castor_FreeAnimation(castor_Animation *anim)
{
    if (anim->texture) {
        SDL_DestroyTexture(anim->texture);
        anim->texture = NULL;
    }
}
