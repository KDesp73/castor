#include "context.h"
#include "raycaster.h"
#include "textures.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

bool ConstructRenderer(Context* ctx)
{
    ctx->window = SDL_CreateWindow(
            ctx->game_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ctx->screen_width, ctx->screen_height, SDL_WINDOW_SHOWN);

    if(!ctx->window) {
        fprintf(stderr, "Could not create window\n");
        return false;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);

    if(!ctx->renderer) {
        fprintf(stderr, "Could not create renderer\n");
        return false;
    }

    return true;
}

void ContextFree(Context* ctx)
{
    PlayerFree(&ctx->player);
    FreeTextures(ctx);

    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
}

void FreeTextures(Context* ctx)
{
    for(size_t i = 0; i < MAX_MAP_WIDTH; i++) {
        if(ctx->textures[i] != NULL)
            SDL_DestroyTexture(ctx->textures[i]); 
    }
    if(ctx->ceiling_texture != NULL)
        SDL_DestroyTexture(ctx->ceiling_texture);
    if(ctx->floor_texture != NULL)
        SDL_DestroyTexture(ctx->floor_texture);
}

