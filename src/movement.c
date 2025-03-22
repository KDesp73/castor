#include "movement.h"
#include "context.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

void MoveFront(Context* ctx, float deltaTime)
{
    float moveX = cos(ctx->level.player->angleX * M_PI / 180.0);
    float moveY = sin(ctx->level.player->angleX * M_PI / 180.0);

    float length = sqrt(moveX * moveX + moveY * moveY);
    moveX /= length;
    moveY /= length;

    moveX *= ctx->level.player->speed * deltaTime;
    moveY *= ctx->level.player->speed * deltaTime;

    if (!CheckCollision(ctx->level.player->X + moveX, ctx->level.player->Y + moveY, ctx)) {
        ctx->level.player->X += moveX;
        ctx->level.player->Y += moveY;
    }
}

void MoveBack(Context* ctx, float deltaTime)
{
    float moveX = cos(ctx->level.player->angleX * M_PI / 180.0) * -1;
    float moveY = sin(ctx->level.player->angleX * M_PI / 180.0) * -1;

    float length = sqrt(moveX * moveX + moveY * moveY);
    moveX /= length;
    moveY /= length;

    moveX *= ctx->level.player->speed * deltaTime;
    moveY *= ctx->level.player->speed * deltaTime;

    if (!CheckCollision(ctx->level.player->X + moveX, ctx->level.player->Y + moveY, ctx)) {
        ctx->level.player->X += moveX;
        ctx->level.player->Y += moveY;
    }
}

void MoveLeft(Context* ctx, float deltaTime)
{
    float moveX = cos((ctx->level.player->angleX - 90) * M_PI / 180.0);
    float moveY = sin((ctx->level.player->angleX - 90) * M_PI / 180.0);

    float length = sqrt(moveX * moveX + moveY * moveY);
    moveX /= length;
    moveY /= length;

    moveX *= ctx->level.player->speed * deltaTime;
    moveY *= ctx->level.player->speed * deltaTime;

    if (!CheckCollision(ctx->level.player->X + moveX, ctx->level.player->Y + moveY, ctx)) {
        ctx->level.player->X += moveX;
        ctx->level.player->Y += moveY;
    }
}

void MoveRight(Context* ctx, float deltaTime)
{
    float moveX = cos((ctx->level.player->angleX + 90) * M_PI / 180.0);
    float moveY = sin((ctx->level.player->angleX + 90) * M_PI / 180.0);

    float length = sqrt(moveX * moveX + moveY * moveY);
    moveX /= length;
    moveY /= length;

    moveX *= ctx->level.player->speed * deltaTime;
    moveY *= ctx->level.player->speed * deltaTime;

    if (!CheckCollision(ctx->level.player->X + moveX, ctx->level.player->Y + moveY, ctx)) {
        ctx->level.player->X += moveX;
        ctx->level.player->Y += moveY;
    }
}

void RotateX(Context* ctx, double delta, float deltaTime)
{
    ctx->level.player->angleX += delta * deltaTime;
    if (ctx->level.player->angleX >= 360) ctx->level.player->angleX -= 360;
}

#define ANGLE_Y_CUTOFF 100
void RotateY(Context *ctx, double delta, float deltaTime)
{
    ctx->level.player->angleY -= delta * deltaTime;

    if (ctx->level.player->angleY < -ANGLE_Y_CUTOFF)
        ctx->level.player->angleY = -ANGLE_Y_CUTOFF;
    else if (ctx->level.player->angleY > ANGLE_Y_CUTOFF)
        ctx->level.player->angleY = ANGLE_Y_CUTOFF;
}

bool CheckCollision(float newX, float newY, const Context* ctx)
{
    // Check collision with map tiles
    int mapX = (int)newX;
    int mapY = (int)newY;

    if (mapX < 0 || mapX >= ctx->level.map_width || mapY < 0 || mapY >= ctx->level.map_height) {
        return true;  // Collision with map boundaries
    }

    int tile = ctx->level.map[mapY][mapX];
    if (tile > 0) {
        // Allow some error margin in collision with tiles
        if (COLLISION_TOLERANCE > 0) {
            float tileCenterX = (float)mapX + 0.5f;
            float tileCenterY = (float)mapY + 0.5f;

            float dx = newX - tileCenterX;
            float dy = newY - tileCenterY;
            float distanceSquared = dx * dx + dy * dy;

            // Check if the distance is less than the error margin
            if (distanceSquared < COLLISION_TOLERANCE * COLLISION_TOLERANCE) {
                return false;  // Allow movement with an error margin
            }
        }
        return true;  // Collision with a solid tile (without margin)
    }

    // Check collision with sprites
    for (int i = 0; i < ctx->level.sprite_count; i++) {
        Sprite* sprite = ctx->level.sprites[i];

        // Skip sprites without collision
        if (!sprite->collision) continue;

        // Calculate distance between the new position and sprite
        float dx = newX - sprite->x;
        float dy = newY - sprite->y;
        float distanceSquared = dx * dx + dy * dy;

        // Define a collision radius (adjust as needed)
        float collisionRadius = 0.9f;  // You can tweak this for each sprite

        // Allow some error margin for sprite collisions
        if (distanceSquared < (collisionRadius - COLLISION_TOLERANCE) * (collisionRadius - COLLISION_TOLERANCE)) {
            return true;  // Collision with a sprite, considering error margin
        }
    }

    return false;  // No collision
}
