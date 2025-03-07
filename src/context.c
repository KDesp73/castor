#include "context.h"
#include "map.h"
#include "textures.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string.h>

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
    ctx->texture_width = 64;
    ctx->texture_height = 64;
    ctx->mouse_sensitivity = 0.25;
    ctx->mouse_inverted = true;
}

void ContextFree(Context* ctx)
{
    PlayerFree(&ctx->player);
    FreeTextures(ctx);
    MapFree(ctx->map, ctx->map_height);

    SDL_DestroyRenderer(ctx->renderer);
    ctx->renderer = NULL;
    SDL_DestroyWindow(ctx->window);
    ctx->window = NULL;
}

void FreeTextures(Context* ctx)
{
    for(size_t i = 0; i < MAX_TEXTURES; i++) {
        if(ctx->textures[i] != NULL) {
            SDL_DestroyTexture(ctx->textures[i]); 
            ctx->textures[i] = NULL;
        }
    }
    ctx->textures_loaded = false;
}

void LoadLevel(Context* ctx, const char* path)
{
    ctx->map = MapLoad(&ctx->map_height, &ctx->map_width, path);
}

void LoadTextures(Context* ctx, const char* path)
{
    TexturesLoad(ctx->renderer, ctx->textures, path);
    ctx->textures_loaded = true;
}
