#include "core.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int test_nav_cache_matches_export(void)
{
    const char* path = "/tmp/castor_test_nav.lvl";
    castor_Map* map = castor_MapCreate(4, 4);
    assert(map != NULL);
    map->grid[0][0] = 1;
    map->grid[1][1] = 0;
    assert(castor_MapSave(map, path));
    castor_MapFree(&map);

    castor_Context ctx = {0};
    castor_ContextInit(&ctx);
    castor_LoadLevelMap(&ctx, path);

    const castor_Map* cached = castor_GetNavMap(&ctx);
    castor_Map* exported = castor_ExportSearchMap(&ctx);
    assert(cached != NULL && exported != NULL);

    for (size_t j = 0; j < ctx.level.map->h; j++) {
        for (size_t i = 0; i < ctx.level.map->w; i++) {
            assert(cached->grid[j][i] == exported->grid[j][i]);
        }
    }

    castor_InvalidateNavMap(&ctx);
    assert(ctx.level.nav_map_dirty);
    const castor_Map* rebuilt = castor_GetNavMap(&ctx);
    assert(rebuilt != NULL);
    assert(rebuilt->grid[0][0] == 1);

    castor_MapFree(&exported);
    castor_ContextFree(&ctx);
    remove(path);
    return 0;
}
