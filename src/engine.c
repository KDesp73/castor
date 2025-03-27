#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


bool castor_EngineInit(castor_Context* ctx)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return false;
    }

    if (!castor_ConstructRenderer(ctx)) {
        fprintf(stderr, "Renderer initialization failed\n");
        return 1;
    }

    return true;
}

void castor_EngineClose(castor_Context* ctx)
{
    castor_ContextFree(ctx);
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

