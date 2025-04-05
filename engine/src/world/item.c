#include "world.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


castor_Item* castor_ItemNew(castor_Sprite* sprite, const char* id)
{
    assert(sprite);
    assert(id && strlen(id) > 0);

    castor_Item* res = malloc(sizeof(castor_Item));

    res->sprite = sprite;
    res->baseZ = sprite->z;
    res->index = -1;
    strncpy(res->id, id, 15);

    return res;
}

void castor_ItemFree(castor_Item** i)
{
    if(*i){
        free(*i);
        *i = NULL;
    }
}

