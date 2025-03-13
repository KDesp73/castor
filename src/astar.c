#include "entity.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_PATH 256
#define INF 1e9

typedef struct {
    int x, y;
} Node;

typedef struct {
    Node node;
    float g, h, f;
} AStarNode;

typedef struct {
    AStarNode nodes[MAX_PATH];
    size_t size;
} PriorityQueue;

// Priority queue helper functions
void PQ_Push(PriorityQueue* pq, AStarNode newNode) {
    if (pq->size >= MAX_PATH) return;  // Prevent overflow

    pq->nodes[pq->size++] = newNode;

    // Sort based on f-cost (lowest first)
    for (size_t i = pq->size - 1; i > 0; i--) {
        if (pq->nodes[i].f < pq->nodes[i - 1].f) {
            AStarNode temp = pq->nodes[i];
            pq->nodes[i] = pq->nodes[i - 1];
            pq->nodes[i - 1] = temp;
        } else {
            break;
        }
    }
}

AStarNode PQ_Pop(PriorityQueue* pq) {
    if (pq->size == 0) return (AStarNode){{-1, -1}, INF, INF, INF};

    AStarNode top = pq->nodes[0];
    for (size_t i = 1; i < pq->size; i++) {
        pq->nodes[i - 1] = pq->nodes[i];
    }
    pq->size--;
    return top;
}

// A* pathfinding function
void AStar(Entity* entity, const Player* player, int** map, size_t mapW, size_t mapH) {
    int startX = (int)floor(entity->x);
    int startY = (int)floor(entity->y);
    int targetX = (int)floor(player->X);
    int targetY = (int)floor(player->Y);

    if (startX == targetX && startY == targetY) return;  // Already at the target

    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    float g_cost[MAX_PATH][MAX_PATH];
    Node came_from[MAX_PATH][MAX_PATH];

    for (size_t i = 0; i < MAX_PATH; i++) {
        for (size_t j = 0; j < MAX_PATH; j++) {
            g_cost[i][j] = INF;
        }
    }

    PriorityQueue openSet = {0};
    AStarNode startNode = {{startX, startY}, 0, abs(targetX - startX) + abs(targetY - startY), 0};
    PQ_Push(&openSet, startNode);
    g_cost[startY][startX] = 0;

    while (openSet.size > 0) {
        AStarNode current = PQ_Pop(&openSet);
        int x = current.node.x;
        int y = current.node.y;

        if (x == targetX && y == targetY) break;  // Path found

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx < 0 || ny < 0 || nx >= mapW || ny >= mapH || map[ny][nx] == 1) continue;  // Out of bounds or obstacle

            float new_g = g_cost[y][x] + 1;
            if (new_g < g_cost[ny][nx]) {
                g_cost[ny][nx] = new_g;
                came_from[ny][nx] = (Node){x, y};
                float h = abs(targetX - nx) + abs(targetY - ny);
                PQ_Push(&openSet, (AStarNode){{nx, ny}, new_g, h, new_g + h});
            }
        }
    }

    // Reconstruct path
    int pathX = targetX, pathY = targetY;
    while (came_from[pathY][pathX].x != startX || came_from[pathY][pathX].y != startY) {
        int tempX = came_from[pathY][pathX].x;
        int tempY = came_from[pathY][pathX].y;
        pathX = tempX;
        pathY = tempY;
    }

    // Move entity toward first step of the path
    entity->x += (pathX - startX) * entity->speed;
    entity->y += (pathY - startY) * entity->speed;
}

