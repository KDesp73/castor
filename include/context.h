#ifndef CONTEXT_H
#define CONTEXT_H

#include "player.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define MAX_MAP_WIDTH  64
#define MAX_MAP_HEIGHT 64
#define MAX_TEXTURES 512

#define DEFAULT_SCREEN_WIDTH 1280
#define DEFAULT_SCREEN_HEIGHT 720

typedef struct {
    const char* game_name;

    // Engine
    bool running;
    float frame_start, frame_time; 
    Player* player;
    int** map;
    size_t map_width;
    size_t map_height;

    // Settings
    int fov;
    bool fullscreen;
    double mouse_sensitivity;
    bool mouse_inverted;

    // SDL
    int screen_width, screen_height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[MAX_TEXTURES];
    bool textures_loaded;
    size_t floor_texture_index;
    size_t ceiling_texture_index;
    size_t texture_width;
    size_t texture_height;

    bool enable_fog;
    float fog_distance;
} Context;


bool ConstructRenderer(Context* ctx);

void ContextInit(Context* ctx);
void ContextFree(Context* ctx);

void FreeTextures(Context* ctx);

void LoadLevel(Context* ctx, const char* path);
void LoadTextures(Context* ctx, const char* path);

#endif // CONTEXT_H
