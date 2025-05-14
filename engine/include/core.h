#ifndef CASTOR_CORE_H
#define CASTOR_CORE_H

//=== Standard Library ===//
#include <stdbool.h>
#include <time.h>

//=== SDL ===//
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

//=== Castor Headers ===//
#include "asset.h"
#include "player.h"
#include "ui.h"
#include "world.h"

//=== Constants ===//
#define MAX_MAP_WIDTH  64
#define MAX_MAP_HEIGHT 64

#define DEFAULT_SCREEN_WIDTH 1280
#define DEFAULT_SCREEN_HEIGHT 720

#define MAX_TEXTURES 32
#define MAX_SPRITES 32
#define MAX_ENTITIES 16
#define MAX_ITEMS 32
#define MAX_EVENTS 128
#define MAX_SOUND_THREADS 32

#define COLLISION_TOLERANCE 0.1f

#ifndef TARGET_FPS
    #define TARGET_FPS 60
#endif
#define FRAME_DELAY ((float)1000 / TARGET_FPS)

//=== Engine Context ===//
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

    // Raycasting
    struct {
        float* z_buffer;
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
        castor_Map* map;
        castor_Sprite* sprites[MAX_SPRITES];
        size_t sprite_count;
        castor_Entity* entities[MAX_ENTITIES];
        size_t entity_count;
        castor_Item* items[MAX_ITEMS];
        size_t item_count;
        castor_Event* events[MAX_EVENTS];
        size_t event_count;
    } level;

    // Sound
    struct {
        int volume;
        castor_SoundThread threads[MAX_SOUND_THREADS];
        size_t thread_count;
    } sound;

    // UI
    UI ui;

} castor_Context;

typedef void (*castor_LevelLoader)(castor_Context*);

//=== SDL ===//
bool castor_ConstructRenderer(castor_Context* ctx);

//=== Raycaster ===//
// FIXME: not clear where this belongs
void castor_FreeTextures(castor_Context* ctx);
void castor_FreeSprites(castor_Context* ctx);
void castor_LoadTextures(castor_Context* ctx);
void castor_LoadSprites(castor_Context* ctx);
void castor_AppendSprite(castor_Context* ctx, castor_Sprite* sprite);
void castor_RemoveSprite(castor_Context* ctx, const castor_Sprite* sprite);

//=== Level ===//
// Move to world module
void castor_FreeEntities(castor_Context* ctx);
void castor_FreeItems(castor_Context* ctx);
void castor_FreeEvents(castor_Context* ctx);
void castor_LoadLevelMap(castor_Context* ctx, const char* path);
void castor_AppendEntity(castor_Context* ctx, castor_Entity* entity);
void castor_AppendItem(castor_Context* ctx, castor_Item* item);
void castor_AppendEvent(castor_Context* ctx, castor_Event* evt);
void castor_FreeLevel(castor_Context* ctx);

castor_Map* castor_ExportSearchMap(castor_Context* ctx);
void castor_UpdateEntities(castor_Context* ctx, float deltaTime);
void castor_ProcessEvents(castor_Context* ctx);
castor_Event* castor_SearchEvent(castor_Context* ctx, const char* id);

void castor_RemoveEntity(castor_Context* ctx, const castor_Entity* entity);
void castor_RemoveItem(castor_Context* ctx, const castor_Item* item);
void castor_ItemsIdle(castor_Context* ctx, float elapsedTime);

//=== Movement & Collision ===//
// TODO: Move to player module
bool castor_CheckCollision(float newX, float newY, const castor_Context* ctx);
void castor_MoveFront(castor_Context* ctx, float deltaTime);
void castor_MoveBack(castor_Context* ctx, float deltaTime);
void castor_MoveLeft(castor_Context* ctx, float deltaTime);
void castor_MoveRight(castor_Context* ctx, float deltaTime);
void castor_RotateX(castor_Context* ctx, double delta, float deltaTime);
void castor_RotateY(castor_Context* ctx, double delta, float deltaTime);

//=== Sound ===//
// TODO: move to asset module
int castor_PlaySound(castor_Context* ctx, const char* file, int volume, Uint32 duration_ms);
void castor_CleanupThreads(castor_Context* ctx);

//=== Engine ===//
void castor_ContextInit(castor_Context* ctx);
void castor_ContextFree(castor_Context* ctx);
bool castor_EngineInit(castor_Context* ctx);
void castor_EngineClose(castor_Context* ctx);

//=== Defined by the developer ===//
extern void presetup(castor_Context* ctx);
extern void setup(castor_Context* ctx);
extern void loop(castor_Context* ctx);
extern void cleanup(castor_Context* ctx);

//=== Timing Macros ===//
#define FPS_START(ctx) \
    Uint32 currentTicks = SDL_GetTicks(); \
    float deltaTime = (currentTicks - ctx->engine.frame_start) / 1000.0f; \
    if (deltaTime > 0.05f) deltaTime = 0.05f; \
    ctx->engine.frame_start = currentTicks;

#define FPS_END(ctx) \
    ctx->engine.frame_time = SDL_GetTicks() - ctx->engine.frame_start; \
    if (ctx->engine.frame_time < FRAME_DELAY) { \
        SDL_Delay(FRAME_DELAY - ctx->engine.frame_time); \
    }

#define DT(ctx) \
    (SDL_GetTicks() - ctx->engine.frame_start) / 1000.0f

#define EVERY_MS(name, interval, code)                     \
    do {                                                   \
        static Uint32 __timer_##name = 0;                  \
        Uint32 __now_##name = SDL_GetTicks();              \
        if ((__now_##name - __timer_##name) >= (interval)) { \
            __timer_##name = __now_##name;                 \
            code;                                          \
        }                                                  \
    } while (0)

//=== Main Function ===//
static inline int castor_Main(int argc, char** argv)
{
    srand(time(NULL));

    castor_Context ctx = {0};
    castor_ContextInit(&ctx);

    presetup(&ctx);

    if (!castor_EngineInit(&ctx)) {
        castor_EngineClose(&ctx);
        return 1;
    }

    setup(&ctx);

    while (ctx.engine.running) {
        loop(&ctx);
    }

    cleanup(&ctx);
    castor_EngineClose(&ctx);
    return 0;
}

#endif // CASTOR_CORE_H
