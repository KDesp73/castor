#include "astar.h"
#include "entity.h"
#include <math.h>

void MoveChase(struct Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime)
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

    EntityMove(entity, entity->x + dirX * entity->speed * deltaTime, entity->y + dirY * entity->speed * deltaTime);
}

void MoveAStar(Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime)
{
    if (!entity || !player) return;

    // Calculate direct distance to the player
    float dx = player->X - entity->x;
    float dy = player->Y - entity->y;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance > entity->detection_range) return; // Player is out of detection range
    if (distance <= MIN_STOP_DISTANCE) return; // Stop moving if too close

    // Get the next step from A*
    Node nextMove = AStar(entity, player, map, mapW, mapH);

    // Calculate movement direction towards the next step
    float moveX = nextMove.x - entity->x;
    float moveY = nextMove.y - entity->y;
    float moveDistance = sqrtf(moveX * moveX + moveY * moveY);

    if (moveDistance > 0) {
        // Normalize movement direction
        float dirX = moveX / moveDistance;
        float dirY = moveY / moveDistance;

        // Apply movement using EntityMove
        EntityMove(entity, entity->x + dirX * entity->speed * deltaTime, entity->y + dirY * entity->speed * deltaTime);
    }
}

void MoveSmoothAStar(Entity* entity, const int** map, size_t mapW, size_t mapH, const Player* player, float deltaTime)
{
    if (!entity || !player) return;

    // Calculate direct distance to the player
    float dx = player->X - entity->x;
    float dy = player->Y - entity->y;
    float distance = sqrtf(dx * dx + dy * dy);

    // If the player is out of the detection range, stop moving
    if (distance > entity->detection_range) return;

    // Stop if too close to the player
    if (distance <= MIN_STOP_DISTANCE) return;

    // Use A* to get the next path node to move towards
    Node nextMove = AStar(entity, player, map, mapW, mapH);

    // Calculate the direction towards the next path node
    float moveX = nextMove.x - entity->x;
    float moveY = nextMove.y - entity->y;
    float moveDistance = sqrtf(moveX * moveX + moveY * moveY);

    if (moveDistance > 0) {
        // Normalize direction
        float dirX = moveX / moveDistance;
        float dirY = moveY / moveDistance;

        // Add a factor to smoothen the movement
        float smoothSpeed = entity->speed * deltaTime;

        // Apply smooth movement using interpolation
        entity->x += dirX * smoothSpeed;
        entity->y += dirY * smoothSpeed;

        // Update sprite position to match entity position
        entity->sprite->x = entity->x;
        entity->sprite->y = entity->y;
    }
}

