#include "world.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_map_roundtrip(void)
{
    const char* path = "/tmp/castor_test_map.lvl";
    castor_Map* map = castor_MapCreate(4, 4);
    assert(map != NULL);

    map->grid[1][1] = 7;
    map->grid[2][2] = 3;

    assert(castor_MapSave(map, path));
    castor_MapFree(&map);

    map = castor_MapLoad(path);
    assert(map != NULL);
    assert(map->w == 4 && map->h == 4);
    assert(map->grid[1][1] == 7);
    assert(map->grid[2][2] == 3);

    castor_MapFree(&map);
    remove(path);
    return 0;
}
