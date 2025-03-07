#ifndef ENGINE_H
#define ENGINE_H

#include "context.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// NOTE: The macros below are used to set an upper limit on the FPS

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif // TARGET_FPS

#define FRAME_DELAY ((float)1000/TARGET_FPS)

#define FPS_START(ctx) ctx->frame_start = SDL_GetTicks()

#define FPS_END(ctx) \
    ctx->frame_time = SDL_GetTicks() - ctx->frame_start; \
    if (ctx->frame_time < FRAME_DELAY) { \
        SDL_Delay(FRAME_DELAY - ctx->frame_time); \
    } 


static inline bool EngineInit(Context* ctx)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
        return false;
    }

    if (!ConstructRenderer(ctx)) {
        fprintf(stderr, "Renderer initialization failed\n");
        return false;
    }

    return true;
}

static inline void EngineClose(Context* ctx)
{
    ContextFree(ctx);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

#endif // ENGINE_H
