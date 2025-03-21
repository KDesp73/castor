#ifndef ITEM_H
#define ITEM_H

#include "sprite.h"
typedef struct {
    char id[16];
    Sprite* sprite;
    float baseZ;
    size_t index;
} Item;

Item* ItemNew(Sprite* sprite, const char* id);
void ItemFree(Item** e);

#endif // ITEM_H
