#include "engine.h"
#include "ui.h"


bool EngineInit(Context* ctx)
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

void EngineClose(Context* ctx)
{
    if (ctx) {
        // First, clean up all resources in the context
        ContextFree(ctx);

        // Now clean up external libraries
        TTF_Quit();
        IMG_Quit();
        
        // Finally, clean up SDL
        SDL_Quit();
    }
}

