#include "context.h"
#include "map.h"
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

void ContextInit(Context* ctx)
{
    ctx->running = true;
    ctx->fov = 60;
    ctx->screen_width = 1080;
    ctx->screen_height = 720;
    ctx->fov = 60;
}

void ContextFree(Context* ctx)
{
    PlayerFree(&ctx->player);
    FreeTextures(ctx);
    MapFree(ctx->map, ctx->map_height);

    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
}

void FreeTextures(Context* ctx)
{
    for(size_t i = 0; i < MAX_MAP_WIDTH; i++) {
        if(ctx->textures[i] != NULL)
            SDL_DestroyTexture(ctx->textures[i]); 
    }
}

void LoadLevel(Context* ctx, const char* path)
{
    ctx->map = MapLoad(&ctx->map_height, &ctx->map_width, path);
}

