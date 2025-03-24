#ifndef ENGINE_H
#define ENGINE_H

#include "context.h"
#include "level.h"
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

bool EngineInit(Context* ctx);
void EngineClose(Context* ctx);

// Defined by the developer
extern void presetup(Context* ctx);
extern void setup(Context* ctx);
extern void loop(Context* ctx);
extern void cleanup(Context* ctx);

static inline int EngineMain(int argc, char** argv)
{
    srand(time(NULL));

    Context ctx = {0};
    ContextInit(&ctx);

    presetup(&ctx);

    if(!EngineInit(&ctx)) {
        EngineClose(&ctx);
        return 1;
    }

    setup(&ctx);

    while (ctx.engine.running) {
        loop(&ctx);
    }

    cleanup(&ctx);

    EngineClose(&ctx);
    return 0;
}

#endif // ENGINE_H
