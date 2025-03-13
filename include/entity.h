#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"
#include "sprite.h"

#define MIN_STOP_DISTANCE 1.5f

typedef struct Entity {
    Sprite* sprite;
    float x, y;
    float speed;
    size_t health;
    float strength; // 0..1
    float toughness; // 0..1
    size_t detection_range; // number of tiles
    float hitbox;
    void (*move)(struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime);
} Entity;
typedef void (*EntityMovement)(struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime);

Entity* EntityNew(Sprite* sprite, float speed, size_t health, float strength, float toughness, size_t detectionRange, float hitbox, EntityMovement move);
void EntityFree(Entity** e);
void EntityMove(Entity* e, float newX, float newY);

void EntityTakeDamage(Entity* e, size_t damage);
float EntityDealDamage(const Entity* e); // TODO:


// Move functions
void MoveChase(struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime);
void MoveAStar(struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime);
void MoveSmoothAStar(Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime);


#endif // ENTITY_H
