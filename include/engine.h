#ifndef ENGINE_H
#define ENGINE_H

#include "context.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

// NOTE: The macros below are used to set an upper limit on the FPS

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif // TARGET_FPS

#define FRAME_DELAY ((float)1000/TARGET_FPS)

#define FPS_START(ctx) \
    Uint32 currentTicks = SDL_GetTicks(); \
    float deltaTime = (currentTicks - ctx->engine.frame_start) / 1000.0f; \
    if (deltaTime > 0.05f) deltaTime = 0.05f; /* Cap at 50ms (20 FPS min) */ \
    ctx->engine.frame_start = currentTicks;


#define FPS_END(ctx) \
    ctx->engine.frame_time = SDL_GetTicks() - ctx->engine.frame_start; \
    if (ctx->engine.frame_time < FRAME_DELAY) { \
        SDL_Delay(FRAME_DELAY - ctx->engine.frame_time); \
    } 

#define DT(ctx) \
    (SDL_GetTicks() - ctx->engine.frame_start) / 1000.0f

#define EVERY_MS(name, interval, code)                       \
    do {                                                     \
        static Uint32 __timer_##name = 0;                    \
        Uint32 __now_##name = SDL_GetTicks();                \
        if ((__now_##name - __timer_##name) >= (interval)) { \
            __timer_##name = __now_##name;                   \
            code;                                            \
        }                                                    \
    } while (0)

bool castor_EngineInit(castor_Context* ctx);
void castor_EngineClose(castor_Context* ctx);

// Defined by the developer
extern void presetup(castor_Context* ctx);
extern void setup(castor_Context* ctx);
extern void loop(castor_Context* ctx);
extern void cleanup(castor_Context* ctx);

static inline int castor_Main(int argc, char** argv)
{
    srand(time(NULL));

    castor_Context ctx = {0};
    castor_ContextInit(&ctx);

    presetup(&ctx);

    if(!castor_EngineInit(&ctx)) {
        castor_EngineClose(&ctx);
        return 1;
    }

    setup(&ctx);

    while (ctx.engine.running) {
        loop(&ctx);
    }

    cleanup(&ctx);

    castor_EngineClose(&ctx);
    return 0;
}

#endif // ENGINE_H
