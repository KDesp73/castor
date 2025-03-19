#ifndef CONTEXT_H
#define CONTEXT_H

#include "entity.h"
#include "event.h"
#include "item.h"
#include "player.h"
#include "sound.h"
#include "sprite.h"
#include "ui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

#define MAX_MAP_WIDTH  64
#define MAX_MAP_HEIGHT 64


#define DEFAULT_SCREEN_WIDTH 1280
#define DEFAULT_SCREEN_HEIGHT 720

typedef struct {
    const char* game_name;

    // Engine
    bool running;
    float frame_start, frame_time; 
    Player* player;

    // Settings
    int fov;
    bool fullscreen;
    double mouse_sensitivity;
    bool mouse_inverted;

    // SDL
    int screen_width, screen_height;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // RayCasting
    float* z_buffer;
    #define MAX_TEXTURES 32
    SDL_Texture* textures[MAX_TEXTURES];
    bool textures_loaded;
    size_t floor_texture_index;
    size_t ceiling_texture_index;
    size_t texture_width;
    size_t texture_height;
    SDL_Texture* sprite_textures[MAX_TEXTURES];
    bool sprites_loaded;
    float render_distance;
    bool enable_fog;
    float fog_distance;

    // Level
    int** map;
    size_t map_width;
    size_t map_height;
    #define MAX_SPRITES 32
    Sprite* sprites[MAX_SPRITES];
    size_t sprite_count;
    #define MAX_ENTITIES 16
    Entity* entities[MAX_ENTITIES];
    size_t entity_count;
    #define MAX_ITEMS 32
    Item* items[MAX_ITEMS];
    size_t item_count;
    #define MAX_EVENTS 128
    Event* events[MAX_EVENTS];
    size_t event_count;

    // Sound
    #define MAX_SOUND_THREADS 32
    SoundThread sound_threads[MAX_SOUND_THREADS];
    size_t sound_thread_count;

    // UI
    UI ui;
} Context;


bool ConstructRenderer(Context* ctx);

void ContextInit(Context* ctx);
void ContextFree(Context* ctx);

void FreeTextures(Context* ctx);
void FreeSprites(Context* ctx);
void FreeEntities(Context* ctx);
void FreeItems(Context* ctx);
void FreeEvents(Context* ctx);

void LoadLevelMap(Context* ctx, const char* path);
void LoadTextures(Context* ctx);

void AppendSprite(Context* ctx, Sprite* sprite);
void AppendEntity(Context* ctx, Entity* entity);
void AppendItem(Context* ctx, Item* item);
void AppendEvent(Context* ctx, Event* evt);

int** ExportSearchMap(Context* ctx);
void UpdateEntities(Context* ctx, float deltaTime);
void ProcessEvents(Context* ctx);

int PlaySound(Context* ctx, const char* file, int volume, Uint32 duration_ms);
void CleanupThreads(Context* ctx);

void RemoveSprite(Context* ctx, const Sprite* sprite);
void RemoveEntity(Context* ctx, const Entity* entity);
void RemoveItem(Context* ctx, const Item* item);

#endif // CONTEXT_H
