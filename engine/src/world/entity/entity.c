#include "world.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

castor_Entity* castor_EntityNew(
    castor_Sprite* sprite, 
    float speed, 
    size_t health, 
    float strength, 
    float toughness, 
    size_t detectionRange, 
    float hitbox, 
    castor_EntityMovement move,
    const char* id)
{
    assert(sprite);
    assert(strength > 0 && strength < 1);
    assert(toughness > 0 && toughness < 1);
    assert(id && strlen(id) > 0);

    castor_Entity* e = malloc(sizeof(castor_Entity));
    if(!e) {
        fprintf(stderr, "Could not allocate memory for entity\n");
        return NULL;
    }

    e->sprite = sprite;
    e->speed = speed;
    e->health = health;
    e->strength = strength;
    e->toughness = toughness;
    e->detection_range = detectionRange;
    e->hitbox = hitbox;
    e->move = move;
    e->default_path = (castor_Path) {
        .length = 0,
        .index = 0
    };
    e->index = -1;
    strncpy(e->id, id, 15);

    return e;
}

void castor_EntityFree(castor_Entity** e)
{
    if(*e){
        free(*e);
        *e = NULL;
    }
}

float castor_EntityTakeDamage(castor_Entity* e, size_t damage)
{
    if(e->health - (1 - e->toughness) * damage < 0) {
        e->health = 0;
        return INF;
    }

    float damageTaken = damage - e->toughness * damage;
    e->health -= damageTaken;
    return damageTaken;
}

#define BASE_DAMAGE 20
float castor_EntityDealDamage(const castor_Entity* e)
{
    return e->strength * BASE_DAMAGE;
}

#include <stdio.h>
#include <math.h>

void castor_EntityFollowDefaultPath(castor_Entity* entity, float deltaTime) {
    // Check if entity pointer is valid
    if (entity == NULL) {
        fprintf(stderr, "Error: castor_Entity pointer is null.\n");
        return;
    }

    // Check if default path is valid
    if (entity->default_path.length <= 0 || entity->default_path.length > MAX_PATH_LENGTH) {
        fprintf(stderr, "Error: Default path is not initialized or has invalid length.\n");
        return;
    }

    // Check if index is within bounds
    if (entity->default_path.index < 0 || entity->default_path.index >= entity->default_path.length) {
        entity->default_path.index = 0;
    }

    // Get target position from the path
    float targetX = entity->default_path.path[entity->default_path.index][0];
    float targetY = entity->default_path.path[entity->default_path.index][1];

    // Calculate direction and distance to the target
    float dx = targetX - entity->sprite->x;
    float dy = targetY - entity->sprite->y;
    float distance = sqrtf(dx * dx + dy * dy);

    // Prevent division by zero
    if (distance > 0) {
        float dirX = dx / distance;
        float dirY = dy / distance;

        entity->sprite->x += dirX * entity->speed * deltaTime;
        entity->sprite->y += dirY * entity->speed * deltaTime;

        if (distance <= entity->speed * deltaTime) {
            entity->default_path.index = (entity->default_path.index + 1) % entity->default_path.length;
        }
    }
}
