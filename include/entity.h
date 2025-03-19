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
} Path;
Path GenerateRandomLoopPath(int** map, size_t mapW, size_t mapH, size_t targetLength, float startX, float startY);
void PathPrint(const Path path);


#define MIN_STOP_DISTANCE 1.5f

#define MOVEMENT_DEFINITION \
    struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime
#define DECLARE_MOVEMENT(x) \
    void x(MOVEMENT_DEFINITION)

typedef struct Entity {
    Sprite* sprite;
    float speed;
    int health;
    float strength; // 0..1
    float toughness; // 0..1
    size_t detection_range; // number of tiles
    float hitbox;
    void (*move)(MOVEMENT_DEFINITION);
    Path default_path;
    size_t index;
} Entity;
typedef void (*EntityMovement)(MOVEMENT_DEFINITION);

Entity* EntityNew(
    Sprite* sprite, 
    float speed, 
    size_t health, 
    float strength, 
    float toughness, 
    size_t detectionRange, 
    float hitbox, 
    EntityMovement move
);
void EntityFree(Entity** e);
void EntityFollowDefaultPath(Entity* entity, float deltaTime);

void EntityTakeDamage(Entity* e, size_t damage);
float EntityDealDamage(const Entity* e);


// Move functions
DECLARE_MOVEMENT(MoveChase);
DECLARE_MOVEMENT(MoveAStar);
DECLARE_MOVEMENT(MoveSmoothAStar);

typedef struct {
    float x, y;          // Position on screen
    float vx, vy;        // Velocity for floating effect
    int damage;          // Damage amount
    float angle;         // Random angle
    float scale;         // Random size
    Uint32 spawn_time;   // Time when spawned
    Uint32 lifetime;     // Duration in ms
} DamageNumber;

#endif // ENTITY_H
