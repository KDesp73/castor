#ifndef A_STAR_H
#define A_STAR_H

#include "entity.h"

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

Node AStar(Entity* entity, const Player* player, const int** map, size_t mapW, size_t mapH);


#endif // A_STAR_H
