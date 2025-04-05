#ifndef A_STAR_H
#define A_STAR_H

#include "entity.h"

#define MAX_PATH 256
#define INF 1e9

typedef struct {
    int x, y;
} castor_Node;

typedef struct {
    castor_Node node;
    float g, h, f;
} castor_AStarNode;

typedef struct {
    castor_AStarNode nodes[MAX_PATH];
    size_t size;
} castor_PriorityQueue;

castor_Node castor_AStar(castor_Entity* entity, const castor_Player* player, const int** map, size_t mapW, size_t mapH);


#endif // A_STAR_H
