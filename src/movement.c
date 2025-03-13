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
    ctx->player->angleY -= delta;

    if (ctx->player->angleY < -ANGLE_Y_CUTOFF)
        ctx->player->angleY = -ANGLE_Y_CUTOFF;
    else if (ctx->player->angleY > ANGLE_Y_CUTOFF)
        ctx->player->angleY = ANGLE_Y_CUTOFF;
}

bool CheckCollision(float newX, float newY, const Context* ctx)
{
    // Check collision with map tiles
    int mapX = (int)newX;
    int mapY = (int)newY;

    if (mapX < 0 || mapX >= ctx->map_width || mapY < 0 || mapY >= ctx->map_height) {
        return true;  // Collision with map boundaries
    }

    int tile = ctx->map[mapY][mapX];
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
    for (int i = 0; i < ctx->sprite_count; i++) {
        Sprite* sprite = ctx->sprites[i];

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

    for(size_t i = 4; i < 83; i++)
        IF_PRESSED_RETURN(i);

    return 0;
}
