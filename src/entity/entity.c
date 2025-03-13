#include "entity.h"
#include "sprite.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Entity* EntityNew(Sprite* sprite, float speed, size_t health, float strength, float toughness, size_t detectionRange, float hitbox, EntityMovement move)
{
    assert(sprite);
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

void EntityMove(Entity* e, float newX, float newY)
{
    e->x = newX + 0.5;
    e->y = newY + 0.5;
    e->sprite->x = newX + 0.5;
    e->sprite->y = newY + 0.5;
}

void EntityTakeDamage(Entity* e, size_t damage)
{
    if(e->health - (1 - e->toughness) * damage < 0) {
        e->health = 0;
        return;
    }

    e->health -= (1 - e->toughness) * damage;
}

