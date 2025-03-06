#ifndef ENGINE_H
#define ENGINE_H

#include "context.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

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
