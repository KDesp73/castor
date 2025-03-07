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

bool EngineInit(Context* ctx);
void EngineClose(Context* ctx);

#endif // ENGINE_H
