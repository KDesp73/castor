#include "item.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


Item* ItemNew(Sprite* sprite, const char* id)
{
    assert(sprite);
    assert(id && strlen(id) > 0);

    Item* res = malloc(sizeof(Item));

    res->sprite = sprite;
    res->baseZ = sprite->z;
    res->index = -1;
    strncpy(res->id, id, 16);

    return res;
}

void ItemFree(Item** i)
{
    if(*i){
        free(*i);
        *i = NULL;
    }
}

