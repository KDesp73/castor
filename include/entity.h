#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"
#include "sprite.h"


typedef struct Entity {
    Sprite* sprite;
    float x, y;
    float speed;
    size_t health;
    float strength; // 0..1
    float toughness; // 0..1
    size_t detection_range; // number of tiles
    float hitbox;
    void (*move)(struct Entity* entity, const int** map, const Player* player, float deltaTime);
} Entity;
typedef void (*EntityMovement)(struct Entity* entity, const int** map, const Player* player, float deltaTime);

Entity* EntityNew(Sprite* sprite, float speed, size_t health, float strength, float toughness, size_t detectionRange, float hitbox, EntityMovement move);
void EntityFree(Entity** e);

void EntityTakeDamage(Entity* e, size_t damage);


// Move functions
void ChasePlayer(struct Entity* entity, const int** map, const Player* player, float deltaTime);

void AStar(Entity* entity, const Player* player, int** map, size_t mapW, size_t mapH);

#endif // ENTITY_H
