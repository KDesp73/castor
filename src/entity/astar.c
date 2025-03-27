#include "astar.h"
#include <math.h>
#include <stdlib.h>

// A* heuristic function (Manhattan distance)
static float Heuristic(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

// Helper function to check if a node is within bounds and not a wall
static bool IsValidNode(int x, int y, size_t mapW, size_t mapH, const int** map) {
    return x >= 0 && y >= 0 && x < mapW && y < mapH && map[y][x] == 0;  // 0 indicates walkable terrain
}

// A* algorithm to find the next step toward the player
castor_Node castor_AStar(castor_Entity* entity, const castor_Player* player, const int** map, size_t mapW, size_t mapH)
{
    int startX = (int)floor(entity->sprite->x);
    int startY = (int)floor(entity->sprite->y);
    int targetX = (int)floor(player->X);
    int targetY = (int)floor(player->Y);

    if (startX == targetX && startY == targetY) {
        return (castor_Node){startX, startY};  // Already at the target, return current position
    }

    const int dx[4] = {1, -1, 0, 0};  // Directions for neighboring nodes (right, left, down, up)
    const int dy[4] = {0, 0, 1, -1};

    // Cost and came_from tracking
    float g_cost[MAX_PATH][MAX_PATH];
    castor_Node came_from[MAX_PATH][MAX_PATH];
    for (size_t i = 0; i < MAX_PATH; i++) {
        for (size_t j = 0; j < MAX_PATH; j++) {
            g_cost[i][j] = INF;
        }
    }

    castor_PriorityQueue openSet = {0};
    castor_AStarNode startNode = {{startX, startY}, 0, Heuristic(startX, startY, targetX, targetY), 0};
    openSet.nodes[openSet.size++] = startNode;
    g_cost[startY][startX] = 0;

    while (openSet.size > 0) {
        // Get the node with the lowest f value
        castor_AStarNode current = openSet.nodes[0];
        size_t bestIdx = 0;
        for (size_t i = 1; i < openSet.size; i++) {
            if (openSet.nodes[i].f < current.f) {
                current = openSet.nodes[i];
                bestIdx = i;
            }
        }

        // Remove the best node from the open set
        for (size_t i = bestIdx; i < openSet.size - 1; i++) {
            openSet.nodes[i] = openSet.nodes[i + 1];
        }
        openSet.size--;

        int x = current.node.x;
        int y = current.node.y;

        if (x == targetX && y == targetY) {
            break;  // Path found
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (!IsValidNode(nx, ny, mapW, mapH, map)) continue;  // Skip invalid nodes

            float tentative_g = g_cost[y][x] + 1;
            if (tentative_g < g_cost[ny][nx]) {
                came_from[ny][nx] = (castor_Node){x, y};
                g_cost[ny][nx] = tentative_g;
                float h = Heuristic(nx, ny, targetX, targetY);
                float f = tentative_g + h;
                castor_AStarNode neighborNode = {{nx, ny}, tentative_g, h, f};
                openSet.nodes[openSet.size++] = neighborNode;
            }
        }
    }

    // Reconstruct the next best move
    castor_Node current = {targetX, targetY};
    castor_Node nextMove = {startX, startY};  // Default to staying in place

    while (came_from[current.y][current.x].x != startX || came_from[current.y][current.x].y != startY) {
        nextMove = current;
        current = came_from[current.y][current.x];
    }

    return nextMove;  // Return the first step toward the player
}
