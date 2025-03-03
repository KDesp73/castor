#include "context.h"
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

    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
}

bool CheckCollision(float newX, float newY, const Context* ctx)
{
    int mapX = (int)newX;
    int mapY = (int)newY;

    if (mapX < 0 || mapX >= ctx->map_width || mapY < 0 || mapY >= ctx->map_height) {
        return true;
    }

    int tile = ctx->map[mapY][mapX];
    if (tile > 0) {
        return true;
    }

    return false;
}

