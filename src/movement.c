#include "movement.h"
#include "context.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

void MoveFront(Context* ctx)
{
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * ctx->player->speed;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * ctx->player->speed;

    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveBack(Context* ctx)
{
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * -ctx->player->speed;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * -ctx->player->speed;

    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveLeft(Context* ctx)
{
    // Calculate the direction perpendicular to the player's forward direction
    float moveX = cos((ctx->player->angleX - 90) * M_PI / 180.0) * ctx->player->speed;
    float moveY = sin((ctx->player->angleX - 90) * M_PI / 180.0) * ctx->player->speed;

    // Check for collision and move if no collision
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveRight(Context* ctx)
{
    // Calculate the direction perpendicular to the player's forward direction
    float moveX = cos((ctx->player->angleX + 90) * M_PI / 180.0) * ctx->player->speed;
    float moveY = sin((ctx->player->angleX + 90) * M_PI / 180.0) * ctx->player->speed;

    // Check for collision and move if no collision
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void RotateLeft(Context* ctx, double delta)
{
    ctx->player->angleX -= delta;
    if (ctx->player->angleX < 0) ctx->player->angleX += 360;

}
void RotateRight(Context* ctx, double delta)
{
    ctx->player->angleX += delta;
    if (ctx->player->angleX >= 360) ctx->player->angleX -= 360;
}

#define ANGLE_Y_CUTOFF 100
void LookUp(Context *ctx, double delta)
{
    if (ctx->player->angleY - delta < -ANGLE_Y_CUTOFF)
        ctx->player->angleY = -ANGLE_Y_CUTOFF;
    else
        ctx->player->angleY -= delta;
}

void LookDown(Context *ctx, double delta)
{
    if (ctx->player->angleY + delta > ANGLE_Y_CUTOFF)
        ctx->player->angleY = ANGLE_Y_CUTOFF;
    else
        ctx->player->angleY += delta;
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

Uint8 HandleInput(Context* ctx)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    // Lock the cursor in the center of the screen
    SDL_ShowCursor(SDL_FALSE); // Hide the cursor
    SDL_SetRelativeMouseMode(SDL_TRUE); // Make the mouse movement relative to the window

    // Get the relative mouse motion
    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);

    // Rotate the camera based on mouse movement
    if (xrel != 0) {
        // Rotate camera left or right based on x movement
        RotateRight(ctx, xrel * ctx->mouse_sensitivity); // Add sensitivity scaling if needed
    }

    if (yrel != 0) {
        // Look up or down based on y movement
        LookUp(ctx, -yrel * ctx->mouse_sensitivity);
    }

    // Optional: Handle other keyboard inputs for movement
    if (keys[SDL_SCANCODE_W]) {
        MoveFront(ctx);
    }
    if (keys[SDL_SCANCODE_S]) {
        MoveBack(ctx);
    }
    if(keys[SDL_SCANCODE_A]) {
        MoveLeft(ctx);
    }
    if(keys[SDL_SCANCODE_D]) {
        MoveRight(ctx);
    }
    if (keys[SDL_SCANCODE_R]) {
        return SDL_SCANCODE_R;
    }

    return 0;
}
