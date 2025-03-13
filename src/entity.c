#include "entity.h"
#include "sprite.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Entity* EntityNew(Sprite* sprite, float speed, size_t health, float strength, float toughness, size_t detectionRange, float hitbox, EntityMovement move)
{
    assert(sprite);
    assert(move);
    assert(strength > 0 && strength < 1);
    assert(toughness > 0 && toughness < 1);

    Entity* e = malloc(sizeof(Entity));
    if(!e) {
        fprintf(stderr, "Could not allocate memory for entity\n");
        return NULL;
    }

    e->sprite = sprite;
    e->x = sprite->x;
    e->y = sprite->y;
    e->speed = speed;
    e->health = health;
    e->strength = strength;
    e->toughness = toughness;
    e->detection_range = detectionRange;
    e->hitbox = hitbox;
    e->move = move;

    return e;
}

void EntityFree(Entity** e)
{
    if(*e){
        free(*e);
        *e = NULL;
    }
}

void EntityTakeDamage(Entity* e, size_t damage)
{
    if(e->health - (1 - e->toughness) * damage < 0) {
        e->health = 0;
        return;
    }

    e->health -= (1 - e->toughness) * damage;
}

#define MIN_STOP_DISTANCE 1.5f
void ChasePlayer(struct Entity* entity, const int** map, const Player* player, float deltaTime)
{
    if (!entity || !player) return;

    // Calculate direction vector to the player
    float dx = player->X - entity->x;
    float dy = player->Y - entity->y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > entity->detection_range) return;

    // Stop if too close to the player
    if (distance <= MIN_STOP_DISTANCE) return;

    // Normalize direction
    float dirX = dx / distance;
    float dirY = dy / distance;

    // Move entity toward player
    entity->x += dirX * entity->speed * deltaTime;
    entity->y += dirY * entity->speed * deltaTime;

    entity->sprite->x = entity->x;
    entity->sprite->y = entity->y;
}
