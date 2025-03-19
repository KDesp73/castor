#include "item.h"
#include <stdlib.h>


Item* ItemNew(Sprite* sprite)
{
    Item* res = malloc(sizeof(Item));

    res->sprite = sprite;
    res->index = -1;

    return res;
}

void ItemFree(Item** i)
{
    if(*i){
        free(*i);
        *i = NULL;
    }
}

