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
    struct {
        bool running;
        float frame_start, frame_time; 
    } engine;

    // Settings
    struct {
        int fov;
        bool fullscreen;
        double mouse_sensitivity;
        bool mouse_inverted;
        float render_distance;
        bool enable_fog;
        float fog_distance;
    } settings;

    // SDL
    struct {
        int screen_width, screen_height;
        SDL_Window* window;
        SDL_Renderer* renderer;
    } sdl;

    // RayCasting
    struct {
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
    } raycaster;

    // Level
    struct {
        int index;
        bool next;
        bool fail;

        castor_Player* player;
        int** map;
        size_t map_width;
        size_t map_height;
        #define MAX_SPRITES 32
        castor_Sprite* sprites[MAX_SPRITES];
        size_t sprite_count;
        #define MAX_ENTITIES 16
        castor_Entity* entities[MAX_ENTITIES];
        size_t entity_count;
        #define MAX_ITEMS 32
        castor_Item* items[MAX_ITEMS];
        size_t item_count;
        #define MAX_EVENTS 128
        castor_Event* events[MAX_EVENTS];
        size_t event_count;
    } level;

    // Sound
    struct {
        int volume;
        #define MAX_SOUND_THREADS 32
        castor_SoundThread threads[MAX_SOUND_THREADS];
        size_t thread_count;
    } sound;

    // UI
    UI ui;
} castor_Context;

// SDL
bool castor_ConstructRenderer(castor_Context* ctx);

// Raycaster
void castor_FreeTextures(castor_Context* ctx);
void castor_FreeSprites(castor_Context* ctx);
void castor_LoadTextures(castor_Context* ctx);
void castor_LoadSprites(castor_Context* ctx);
void castor_AppendSprite(castor_Context* ctx, castor_Sprite* sprite);
void castor_RemoveSprite(castor_Context* ctx, const castor_Sprite* sprite);

// Level
void castor_FreeEntities(castor_Context* ctx);
void castor_FreeItems(castor_Context* ctx);
void castor_FreeEvents(castor_Context* ctx);
void castor_LoadLevelMap(castor_Context* ctx, const char* path);
void castor_AppendEntity(castor_Context* ctx, castor_Entity* entity);
void castor_AppendItem(castor_Context* ctx, castor_Item* item);
void castor_AppendEvent(castor_Context* ctx, castor_Event* evt);
int** castor_ExportSearchMap(castor_Context* ctx);
void castor_UpdateEntities(castor_Context* ctx, float deltaTime);
void castor_ProcessEvents(castor_Context* ctx);
castor_Event* castor_SearchEvent(castor_Context* ctx, const char* id);
void castor_RemoveEntity(castor_Context* ctx, const castor_Entity* entity);
void castor_RemoveItem(castor_Context* ctx, const castor_Item* item);
void castor_ItemsIdle(castor_Context* ctx, float elapsedTime);
void castor_FreeLevel(castor_Context* ctx);

// Sound
int castor_PlaySound(castor_Context* ctx, const char* file, int volume, Uint32 duration_ms);
void castor_CleanupThreads(castor_Context* ctx);

// Engine
void castor_ContextInit(castor_Context* ctx);
void castor_ContextFree(castor_Context* ctx);









#endif // CONTEXT_H
