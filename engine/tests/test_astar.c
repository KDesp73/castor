#include "world.h"
#include <assert.h>
#include <math.h>

int test_astar_finds_target(void)
{
    const int size = 5;
    int** grid = malloc((size_t)size * sizeof(int*));
    for (int y = 0; y < size; y++) {
        grid[y] = calloc((size_t)size, sizeof(int));
    }

    castor_Sprite* sprite = castor_SpriteNew(1.5f, 1.5f, 0, 0, 1.0f, false, "test");
    castor_Entity* entity = castor_EntityNew(sprite, 1.0f, 10, 0.5f, 0.5f, 10, 1.0f, castor_MoveAStar, "e1");
    castor_Player* player = castor_PlayerNew(3.5f, 3.5f);

    castor_Node next = castor_AStar(entity, player, (const int**)grid, (size_t)size, (size_t)size);
    assert(next.x >= 0 && next.y >= 0);

    castor_EntityFree(&entity);
    castor_PlayerFree(&player);
    for (int y = 0; y < size; y++) {
        free(grid[y]);
    }
    free(grid);
    return 0;
}
