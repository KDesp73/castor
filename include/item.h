#ifndef ITEM_H
#define ITEM_H

#include "sprite.h"
typedef struct {
    Sprite* sprite;
    size_t index;
} Item;

Item* ItemNew(Sprite* sprite);
void ItemFree(Item** e);

#endif // ITEM_H
