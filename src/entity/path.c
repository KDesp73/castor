#include "entity.h"
#include <stdbool.h>
#include <stdlib.h>

static bool IsWalkable(int** map, int x, int y, size_t mapW, size_t mapH)
{
    if (x < 0 || y < 0 || x >= mapW || y >= mapH) return false;  // Out of bounds
    return map[y][x] == 0;  // 0 is assumed to be a walkable tile, change if needed
}

Path GenerateRandomLoopPath(int** map, size_t mapW, size_t mapH)
{
    // Define starting point for the path
    int startX = rand() % mapW;
    int startY = rand() % mapH;

    // Define current position
    int x = startX;
    int y = startY;

    // Define the path as a list of points (can be a simple array or a linked list)
    int path[MAX_PATH_LENGTH][2];
    size_t pathLength = 0;

    // Store the starting point in the path
    path[pathLength][0] = x;
    path[pathLength][1] = y;
    pathLength++;

    // Directions array (up, right, down, left)
    int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // Up, Right, Down, Left

    // Number of steps before looping or retrying
    int maxSteps = 1000;
    int stepCount = 0;

    // Try to generate a random loop path
    while (stepCount < maxSteps) {
        // Choose a random direction (up, down, left, right)
        int dir = rand() % 4;
        int newX = x + directions[dir][0];
        int newY = y + directions[dir][1];

        // Check if the new point is walkable
        if (IsWalkable(map, newX, newY, mapW, mapH)) {
            // Add the new point to the path
            path[pathLength][0] = newX;
            path[pathLength][1] = newY;
            pathLength++;

            // Move to the new point
            x = newX;
            y = newY;

            // If we've made a full loop (i.e., returned to the start), break the loop
            if (x == startX && y == startY && pathLength > 3) {
                break;  // Path is closed
            }
        }

        stepCount++;
    }

    // Create the path structure to return
    Path result;
    result.length = pathLength;
    for (size_t i = 0; i < pathLength; i++) {
        result.path[i][0] = path[i][0];
        result.path[i][1] = path[i][1];
    }

    return result;  // Return the generated path
}
