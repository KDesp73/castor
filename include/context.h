#ifndef CONTEXT_H
#define CONTEXT_H

#include "player.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define MAX_MAP_WIDTH  64
#define MAX_MAP_HEIGHT 64
typedef struct {
    bool running;

    int screen_width;
    int screen_height;
    int map_width;
    int map_height;
    int fov;
    const char* game_name;

    Player* player;
    int map[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

    SDL_Window* window;
    SDL_Renderer* renderer;
} Context;


void ConstructRenderer(Context* ctx);

void ContextFree(Context* ctx);

void PlayerMoveFront(Context* context);
void PlayerMoveBack(Context* context);
void PlayerRotateLeft(Context* context);
void PlayerRotateRight(Context* context);

bool CheckCollision(float newX, float newY, const Context* ctx);


#endif // CONTEXT_H
