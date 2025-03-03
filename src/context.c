#include "context.h"
#include <SDL2/SDL_video.h>

void ConstructRenderer(Context* ctx)
{
    ctx->window = SDL_CreateWindow(
            ctx->game_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ctx->screen_width, ctx->screen_height, SDL_WINDOW_SHOWN);

    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
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

void PlayerMoveFront(Context* ctx)
{
    // Calculate the player's intended movement
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * ctx->player->speed;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * ctx->player->speed;

    // Check if the new position collides with a wall
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void PlayerMoveBack(Context* ctx)
{
    // Calculate the player's intended movement
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * -ctx->player->speed;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * -ctx->player->speed;

    // Check if the new position collides with a wall
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

#define ANGLE_DELTA 0.5
void PlayerRotateLeft(Context* ctx)
{
    ctx->player->angleX -= ANGLE_DELTA;
    if (ctx->player->angleX < 0) ctx->player->angleX += 360;

}
void PlayerRotateRight(Context* ctx)
{
    ctx->player->angleX += ANGLE_DELTA;
    if (ctx->player->angleX >= 360) ctx->player->angleX -= 360;
}
void PlayerLookUp(Context *ctx)
{
    ctx->player->angleY -= ANGLE_DELTA;
}
void PlayerLookDown(Context *ctx)
{
    ctx->player->angleY += ANGLE_DELTA;
}
