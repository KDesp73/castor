#ifndef EVENTS_H
#define EVENTS_H

#include "context.h"
#include "event.h"
#include "inventory.h"
#include <stdbool.h>

bool PlayerAttackTrigger(Event* evt);
void PlayerAttackAction(Event* evt);

bool PickItemTrigger(Event* evt);
void PickItemAction(Event* evt);

#define DECLARE_DOOR_TRIGGER(num, _x, _y)                      \
    static inline bool Door##num##Trigger(Event* evt)          \
    {                                                          \
        Context* ctx = (evt)->ctx;                             \
        Player* player = ctx->level.player;                    \
        if (!(INV.key)) return false;                          \
        return (                                               \
            ((player->X) >= (_x) && (player->X) < (_x) + 1) && \
            ((player->Y) >= (_y) && (player->Y) < (_y) + 1)    \
        );                                                     \
    }


DECLARE_DOOR_TRIGGER(0, 8, 14);
DECLARE_DOOR_TRIGGER(2, 1, 7);

void DoorTooltipAction(Event* evt);
void DoorKeyAction(Event* evt);

#endif // EVENTS_H
