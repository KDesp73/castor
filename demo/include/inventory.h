#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>
#include <stddef.h>
typedef struct {
    bool key;
    bool glasses;
    bool sword;
} Inventory;

extern Inventory INV;

void InventoryPrint(Inventory inv);
static inline void InventoryClear() { INV = (Inventory){0}; }

#endif // INVENTORY_H
