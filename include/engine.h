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

#define FPS_START(ctx) ctx->engine.frame_start = SDL_GetTicks()

#define FPS_END(ctx) \
    ctx->engine.frame_time = SDL_GetTicks() - ctx->engine.frame_start; \
    if (ctx->engine.frame_time < FRAME_DELAY) { \
        SDL_Delay(FRAME_DELAY - ctx->engine.frame_time); \
    } 

#define EVERY_MS(name, interval, code)                       \
    do {                                                     \
        static Uint32 __timer_##name = 0;                    \
        Uint32 __now_##name = SDL_GetTicks();                \
        if ((__now_##name - __timer_##name) >= (interval)) { \
            __timer_##name = __now_##name;                   \
            code;                                            \
        }                                                    \
    } while (0)

bool EngineInit(Context* ctx);
void EngineClose(Context* ctx);

#endif // ENGINE_H
