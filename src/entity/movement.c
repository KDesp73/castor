#include "astar.h"
#include "entity.h"
#include <math.h>

void castor_MoveChase(struct castor_Entity* entity, const int** map, size_t mapW, size_t mapH, const castor_Player* player, float deltaTime)
{
    if (!entity || !player) return;

    // Calculate direction vector to the player
    float dx = player->X - entity->sprite->x;
    float dy = player->Y - entity->sprite->y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > entity->detection_range) {
        castor_EntityFollowDefaultPath(entity, deltaTime);
        return;
    }

    // Stop if too close to the player
    if (distance <= MIN_STOP_DISTANCE) return;

    // Normalize direction
    float dirX = dx / distance;
    float dirY = dy / distance;

    entity->sprite->x += dirX * entity->speed * deltaTime; 
    entity->sprite->y += dirY * entity->speed * deltaTime;
}

void castor_MoveAStar(castor_Entity* entity, const int** map, size_t mapW, size_t mapH, const castor_Player* player, float deltaTime)
{
    if (!entity || !player) return;

    // Calculate direct distance to the player
    float dx = player->X - entity->sprite->x;
    float dy = player->Y - entity->sprite->y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > entity->detection_range) {
        castor_EntityFollowDefaultPath(entity, deltaTime);
        return;
    }
    if (distance <= MIN_STOP_DISTANCE) return; // Stop moving if too close

    // Get the next step from A*
    castor_Node nextMove = castor_AStar(entity, player, map, mapW, mapH);

    // Calculate movement direction towards the next step
    float moveX = nextMove.x - entity->sprite->x;
    float moveY = nextMove.y - entity->sprite->y;
    float moveDistance = sqrtf(moveX * moveX + moveY * moveY);

    if (moveDistance > 0) {
        float dirX = moveX / moveDistance;
        float dirY = moveY / moveDistance;

        entity->sprite->x += dirX * entity->speed * deltaTime;
        entity->sprite->y += dirY * entity->speed * deltaTime;
    }
}

void castor_MoveSmoothAStar(castor_Entity* entity, const int** map, size_t mapW, size_t mapH, const castor_Player* player, float deltaTime)
{
    if (!entity || !player) return;

    // Calculate direct distance to the player
    float dx = player->X - entity->sprite->x;
    float dy = player->Y - entity->sprite->y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > entity->detection_range) {
        castor_EntityFollowDefaultPath(entity, deltaTime);
        return;
    }

    if (distance <= MIN_STOP_DISTANCE) return;

    castor_Node nextMove = castor_AStar(entity, player, map, mapW, mapH);

    // Calculate the direction towards the next path node
    float moveX = nextMove.x - entity->sprite->x;
    float moveY = nextMove.y - entity->sprite->y;
    float moveDistance = sqrtf(moveX * moveX + moveY * moveY);

    if (moveDistance > 0) {
        float dirX = moveX / moveDistance;
        float dirY = moveY / moveDistance;

        // Add a factor to smoothen the movement
        float smoothSpeed = entity->speed * deltaTime;

        entity->sprite->x += dirX * smoothSpeed;
        entity->sprite->y += dirY * smoothSpeed;
    }
}

