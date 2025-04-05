#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"
#include "sprite.h"
#include <SDL2/SDL_stdinc.h>

#define MAX_PATH_LENGTH 200

typedef struct {
    float path[MAX_PATH_LENGTH][2];
    size_t length;
    int index;
} castor_Path;
castor_Path castor_GenerateRandomLoopPath(int** map, size_t mapW, size_t mapH, size_t targetLength, float startX, float startY);
void castor_PathPrint(const castor_Path path);


#define MIN_STOP_DISTANCE 1.5f

#define MOVEMENT_DEFINITION \
    struct castor_Entity* entity, const int** map, size_t mapW, size_t mapH, const castor_Player* player, float deltaTime
#define DECLARE_MOVEMENT(x) \
    void x(MOVEMENT_DEFINITION)

typedef struct castor_Entity {
    castor_Sprite* sprite;
    float speed;
    int health;
    float strength; // 0..1
    float toughness; // 0..1
    size_t detection_range; // number of tiles
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


// Move functions
DECLARE_MOVEMENT(castor_MoveChase);
DECLARE_MOVEMENT(castor_MoveAStar);
DECLARE_MOVEMENT(castor_MoveSmoothAStar);

typedef struct {
    float x, y;          // Position on screen
    float vx, vy;        // Velocity for floating effect
    int damage;          // Damage amount
    float angle;         // Random angle
    float scale;         // Random size
    Uint32 spawn_time;   // Time when spawned
    Uint32 lifetime;     // Duration in ms
} castor_DamageNumber;

#endif // ENTITY_H
