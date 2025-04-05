#include "world.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool IsWalkable(int** map, int x, int y, size_t mapW, size_t mapH)
{
    if (x < 0 || y < 0 || x >= mapW || y >= mapH) return false;  // Out of bounds
    return map[y][x] == 0;  // 0 is assumed to be a walkable tile, change if needed
}

castor_Path castor_GenerateRandomLoopPath(int** map, size_t mapW, size_t mapH, size_t targetLength, float startX, float startY)
{
    castor_Path result;
    result.length = 0;

    if (startX < 0 || startX >= mapW || startY < 0 || startY >= mapH || !IsWalkable(map, (int)startX, (int)startY, mapW, mapH)) {
        fprintf(stderr, "Error: Starting point (%.2f, %.2f) is invalid or not walkable.\n", startX, startY);
        return result; // Return an empty path
    }

    float x = startX;
    float y = startY;

    bool visited[mapW][mapH];
    for (size_t i = 0; i < mapW; i++) {
        for (size_t j = 0; j < mapH; j++) {
            visited[i][j] = false;
        }
    }

    // Initialize the path with the start position
    result.path[result.length][0] = x;
    result.path[result.length][1] = y;
    result.length++;
    visited[(int)startX][(int)startY] = true;

    int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // Up, Right, Down, Left

    int maxSteps = 1000;
    int stepCount = 0;

    while (stepCount < maxSteps && result.length < targetLength) {
        // Try to move in each of the 4 directions (up, right, down, left)
        bool moved = false;
        for (int i = 0; i < 4; i++) {
            int newX = (int)(x - 0.5f) + directions[i][0]; // Convert to grid coordinates
            int newY = (int)(y - 0.5f) + directions[i][1]; // Convert to grid coordinates

            // Check if the new point is walkable and not visited
            if (IsWalkable(map, newX, newY, mapW, mapH) && !visited[newX][newY]) {
                // Add the new point to the path
                result.path[result.length][0] = (float)newX + 0.5f; // Center the point
                result.path[result.length][1] = (float)newY + 0.5f;
                result.length++;

                // Mark as visited and move to the new point
                visited[newX][newY] = true;
                x = (float)newX + 0.5f;
                y = (float)newY + 0.5f;
                moved = true;
                break;
            }
        }

        // If no valid move, backtrack to the previous position
        if (!moved) {
            if (result.length > 1) {
                // Mark the current position as unvisited
                visited[(int)(x - 0.5f)][(int)(y - 0.5f)] = false;

                // Backtrack
                result.length--;
                x = result.path[result.length - 1][0];
                y = result.path[result.length - 1][1];
            } else {
                // If stuck at the start, reset the path and try again
                result.length = 0;
                result.path[result.length][0] = startX + 0.5f;
                result.path[result.length][1] = startY + 0.5f;
                result.length++;
                for (size_t i = 0; i < mapW; i++) {
                    for (size_t j = 0; j < mapH; j++) {
                        visited[i][j] = false;
                    }
                }
                visited[(int)startX][(int)startY] = true;
                x = startX + 0.5f;
                y = startY + 0.5f;
            }
        }

        stepCount++;
    }

    // Ensure the path ends at the starting point to form a loop
    if (result.length >= 2) {
        float lastX = result.path[result.length - 1][0];
        float lastY = result.path[result.length - 1][1];

        // Check if the last point is adjacent to the starting point
        if (fabsf(lastX - (startX + 0.5f)) + fabsf(lastY - (startY + 0.5f)) == 1.0f) {
            result.path[result.length][0] = startX + 0.5f;
            result.path[result.length][1] = startY + 0.5f;
            result.length++;
        } else {
            // If not adjacent, try to find a path back to the start
            for (int i = 0; i < 4; i++) {
                int newX = (int)(lastX - 0.5f) + directions[i][0];
                int newY = (int)(lastY - 0.5f) + directions[i][1];

                if (newX == (int)startX && newY == (int)startY) {
                    result.path[result.length][0] = startX + 0.5f;
                    result.path[result.length][1] = startY + 0.5f;
                    result.length++;
                    break;
                }
            }
        }
    }

    return result;
}

void castor_PathPrint(const castor_Path path)
{
    for(size_t i = 0; i < path.length; i++){
        printf("%zu: %f, %f %s\n", i, path.path[i][0], path.path[i][1], (i == path.index) ? "<" : "");
    }
}
