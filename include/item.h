#ifndef ITEM_H
#define ITEM_H

#include "sprite.h"
typedef struct {
    char id[16];
    castor_Sprite* sprite;
    float baseZ;
    size_t index;
} castor_Item;

castor_Item* castor_ItemNew(castor_Sprite* sprite, const char* id);
void castor_ItemFree(castor_Item** e);

#endif // ITEM_H
