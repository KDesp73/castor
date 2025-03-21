#include "inventory.h"
#include <stdio.h>

Inventory INV = {0};

void InventoryPrint(Inventory inv)
{
    printf("-INVENTORY-\n");
    if(inv.key)
        printf("Key 🔑\n");
    if(inv.glasses)
        printf("Glasses 🕶\n");
    if(inv.sword)
        printf("Sword 🔪\n");
}
