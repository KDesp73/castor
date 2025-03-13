#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"
#include "sprite.h"

#define MAX_PATH_LENGTH 200
#define MAX_RADIUS 10

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
typedef struct {
    int path[MAX_PATH_LENGTH][2];
    size_t length;
} Path;
Path GenerateRandomLoopPath(int** map, size_t mapW, size_t mapH);


#define MIN_STOP_DISTANCE 1.5f

#define MOVEMENT_DEFINITION \
    struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime
#define DECLARE_MOVEMENT(x) \
    void x(MOVEMENT_DEFINITION)

typedef struct Entity {
    Sprite* sprite;
    float x, y;
    float speed;
    size_t health;
    float strength; // 0..1
    float toughness; // 0..1
    size_t detection_range; // number of tiles
    float hitbox;
    void (*move)(MOVEMENT_DEFINITION);
    Path default_path;
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
void EntityMove(Entity* e, float newX, float newY);

void EntityTakeDamage(Entity* e, size_t damage);
float EntityDealDamage(const Entity* e);


// Move functions
DECLARE_MOVEMENT(MoveChase);
DECLARE_MOVEMENT(MoveAStar);
DECLARE_MOVEMENT(MoveSmoothAStar);


#endif // ENTITY_H
