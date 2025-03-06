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
    double step_size;
    const char* game_name;

    Player* player;
    int map[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[MAX_MAP_WIDTH];
    size_t floor_texture_index;
    size_t ceiling_texture_index;
    size_t texture_width;
    size_t texture_height;
} Context;


bool ConstructRenderer(Context* ctx);

void ContextInit(Context* ctx);
void ContextFree(Context* ctx);

void LoadTextures(Context* ctx);
void FreeTextures(Context* ctx);


#endif // CONTEXT_H
