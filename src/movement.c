#include "movement.h"
#include "context.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

void MoveFront(Context* ctx)
{
    float moveX = cos(ctx->player->angleX * M_PI / 180.0);
    float moveY = sin(ctx->player->angleX * M_PI / 180.0);
    
    // Normalize the vector
    float length = sqrt(moveX * moveX + moveY * moveY);  // Length of the vector
    moveX /= length;  // Normalize
    moveY /= length;  // Normalize

    // Scale by speed
    moveX *= ctx->player->speed;
    moveY *= ctx->player->speed;

    // Check for collision and move if no collision
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveBack(Context* ctx)
{
    float moveX = cos(ctx->player->angleX * M_PI / 180.0) * -1;
    float moveY = sin(ctx->player->angleX * M_PI / 180.0) * -1;

    // Normalize the vector
    float length = sqrt(moveX * moveX + moveY * moveY);  // Length of the vector
    moveX /= length;  // Normalize
    moveY /= length;  // Normalize

    // Scale by speed
    moveX *= ctx->player->speed;
    moveY *= ctx->player->speed;

    // Check for collision and move if no collision
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveLeft(Context* ctx)
{
    // Calculate the perpendicular direction (90 degrees counterclockwise)
    float moveX = cos((ctx->player->angleX - 90) * M_PI / 180.0);
    float moveY = sin((ctx->player->angleX - 90) * M_PI / 180.0);

    // Normalize the vector
    float length = sqrt(moveX * moveX + moveY * moveY);  // Length of the vector
    moveX /= length;  // Normalize
    moveY /= length;  // Normalize

    // Scale by speed
    moveX *= ctx->player->speed;
    moveY *= ctx->player->speed;

    // Check for collision and move if no collision
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void MoveRight(Context* ctx)
{
    // Calculate the perpendicular direction (90 degrees clockwise)
    float moveX = cos((ctx->player->angleX + 90) * M_PI / 180.0);
    float moveY = sin((ctx->player->angleX + 90) * M_PI / 180.0);

    // Normalize the vector
    float length = sqrt(moveX * moveX + moveY * moveY);  // Length of the vector
    moveX /= length;  // Normalize
    moveY /= length;  // Normalize

    // Scale by speed
    moveX *= ctx->player->speed;
    moveY *= ctx->player->speed;

    // Check for collision and move if no collision
    if (!CheckCollision(ctx->player->X + moveX, ctx->player->Y + moveY, ctx)) {
        ctx->player->X += moveX;
        ctx->player->Y += moveY;
    }
}

void RotateX(Context* ctx, double delta)
{
    ctx->player->angleX += delta;
    if (ctx->player->angleX >= 360) ctx->player->angleX -= 360;
}

#define ANGLE_Y_CUTOFF 100
void RotateY(Context *ctx, double delta)
{
    if (ctx->player->angleY - delta < -ANGLE_Y_CUTOFF)
        ctx->player->angleY = -ANGLE_Y_CUTOFF;
    else
        ctx->player->angleY -= delta;
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

#define IF_PRESSED_RETURN(key) \
    if (keys[key]) return key 


Uint8 HandleInput(Context* ctx)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    // Lock the cursor in the center of the screen
    SDL_ShowCursor(SDL_FALSE); // Hide the cursor
    SDL_SetRelativeMouseMode(SDL_TRUE); // Make the mouse movement relative to the window

    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);

    if (xrel != 0) {
        RotateX(ctx, xrel * ctx->mouse_sensitivity);
    }

    if (yrel != 0) {
        RotateY(ctx, ((ctx->mouse_inverted) ? -yrel : yrel) * (ctx->mouse_sensitivity + 0.15)); // More sensitivity on the y-axis
    }

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

#define ANGLE_DELTA 2.5
    if(keys[SDL_SCANCODE_UP]) {
        RotateY(ctx, ANGLE_DELTA * 2);
    }
    if(keys[SDL_SCANCODE_DOWN]) {
        RotateY(ctx, -ANGLE_DELTA * 2);
    }
    if(keys[SDL_SCANCODE_LEFT]) {
        RotateX(ctx, -ANGLE_DELTA);
    }
    if(keys[SDL_SCANCODE_RIGHT]) {
        RotateX(ctx, ANGLE_DELTA);
    }

    IF_PRESSED_RETURN(SDL_SCANCODE_R);
    IF_PRESSED_RETURN(SDL_SCANCODE_ESCAPE);
    IF_PRESSED_RETURN(SDL_SCANCODE_T);

    return 0;
}
