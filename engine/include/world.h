#ifndef CASTOR_WORLD_H
#define CASTOR_WORLD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_stdinc.h>
#include "player.h"

// ================= SPRITE =================
#define SPRITE_UP 0.5
#define SPRITE_MIDDLE 0
#define SPRITE_DOWN -0.5

#define SPRITE_PLACE(_x, _y) \
    .x = (_x) + 0.5, \
    .y = (_y) + 0.5

typedef struct {
    float x, y, z;
    int width, height;
    float distance;
    size_t texture_id;
    float scale;
    bool collision;
    bool is_animation;
    size_t index;
    char id[16];
} castor_Sprite;

castor_Sprite* castor_SpriteNew(float x, float y, float z, size_t textureId, float scale, bool collision, const char* id);
void castor_SpriteFree(castor_Sprite** s);
int castor_SpriteCmp(const void* a, const void* b);

// ================= ENTITY =================
#define MAX_PATH_LENGTH 200
#define MIN_STOP_DISTANCE 1.5f

#define MOVEMENT_DEFINITION \
    struct castor_Entity* entity, const int** map, size_t mapW, size_t mapH, const castor_Player* player, float deltaTime
#define DECLARE_MOVEMENT(x) \
    void x(MOVEMENT_DEFINITION)

typedef struct {
    float path[MAX_PATH_LENGTH][2];
    size_t length;
    int index;
} castor_Path;

castor_Path castor_GenerateRandomLoopPath(int** map, size_t mapW, size_t mapH, size_t targetLength, float startX, float startY);
void castor_PathPrint(const castor_Path path);

typedef struct castor_Entity {
    castor_Sprite* sprite;
    float speed;
    int health;
    float strength;
    float toughness;
    size_t detection_range;
    float hitbox;
    void (*move)(MOVEMENT_DEFINITION);
    castor_Path default_path;
    size_t index;
    char id[16];
} castor_Entity;

typedef void (*castor_EntityMovement)(MOVEMENT_DEFINITION);

castor_Entity* castor_EntityNew(
    castor_Sprite* sprite, 
    float speed, 
    size_t health, 
    float strength, 
    float toughness, 
    size_t detectionRange, 
    float hitbox, 
    castor_EntityMovement move,
    const char* id
);

void castor_EntityFree(castor_Entity** e);
void castor_EntityFollowDefaultPath(castor_Entity* entity, float deltaTime);
float castor_EntityTakeDamage(castor_Entity* e, size_t damage);
float castor_EntityDealDamage(const castor_Entity* e);

// Movement functions
DECLARE_MOVEMENT(castor_MoveChase);
DECLARE_MOVEMENT(castor_MoveAStar);
DECLARE_MOVEMENT(castor_MoveSmoothAStar);

// ================= PATHFINDING =================
#define MAX_PATH 256
#define INF 1e9

typedef struct {
    int x, y;
} castor_Node;

typedef struct {
    castor_Node node;
    float g, h, f;
} castor_AStarNode;

typedef struct {
    castor_AStarNode nodes[MAX_PATH];
    size_t size;
} castor_PriorityQueue;

castor_Node castor_AStar(castor_Entity* entity, const castor_Player* player, const int** map, size_t mapW, size_t mapH);

// ================= DAMAGE NUMBERS =================
typedef struct {
    float x, y;
    float vx, vy;
    int damage;
    float angle;
    float scale;
    Uint32 spawn_time;
    Uint32 lifetime;
} castor_DamageNumber;

// ================= EVENT =================
typedef struct castor_Event{
    bool (*trigger)(struct castor_Event* self);
    void (*action)(struct castor_Event* self);
    void* ctx;
    void* state;
    bool trigger_once;
    bool triggered;
    bool first_run;
    Uint32 cooldown;
    Uint32 last_processed;
    int index;
    char id[16];
} castor_Event;

typedef bool (*EventTrigger)(struct castor_Event* self);
typedef void (*EventAction)(struct castor_Event* self);

castor_Event* castor_EventNew(void* ctx, bool trigger_once, Uint32 cooldown, EventTrigger trigger, EventAction action);
void castor_EventFree(castor_Event** evt);
void castor_EventProcess(castor_Event* evt);

// ================= ITEM =================
typedef struct {
    char id[16];
    castor_Sprite* sprite;
    float baseZ;
    size_t index;
} castor_Item;

castor_Item* castor_ItemNew(castor_Sprite* sprite, const char* id);
void castor_ItemFree(castor_Item** e);

// ================= LEVEL =================
#define castor_LoadLevel(ctx, loader) \
    loader(ctx)

// ================= MAP =================
typedef struct {
    int** grid;
    size_t w, h;
} castor_Map;

void castor_MapPrint(castor_Map map);
castor_Map* castor_MapCreate(size_t rows, size_t cols);
void castor_MapFree(castor_Map** map);
bool castor_MapSave(const castor_Map* map, const char* path);
castor_Map* castor_MapLoad(const char* path);
void castor_MapCpy(castor_Map* src, castor_Map* dst);

#endif // CASTOR_WORLD_H
