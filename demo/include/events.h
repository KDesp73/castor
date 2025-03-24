#ifndef EVENTS_H
#define EVENTS_H

#include "context.h"
#include "event.h"
#include "ingame-ui.h"
#include "inventory.h"
#include <stdbool.h>

bool PlayerAttackTrigger(Event* evt);
void PlayerAttackAction(Event* evt);

bool EnemyAttackTrigger(Event* evt);
void EnemyAttackAction(Event* evt);

bool PickItemTrigger(Event* evt);
void PickItemAction(Event* evt);


bool GlitchTrigger(Event* evt);
#define DECLARE_GLITCH_ACTION(num, code) \
    static inline void Glitch##num##Action(Event* evt) \
    { \
        Context* ctx = evt->ctx; \
        GlitchActivated = true; \
        code \
    }

DECLARE_GLITCH_ACTION(0, {
    ctx->level.map[5][13] = 0;
    ctx->level.map[13][9] = 0;
})


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


DECLARE_DOOR_TRIGGER(0, 7, 14);
DECLARE_DOOR_TRIGGER(1, 9, 1);
DECLARE_DOOR_TRIGGER(2, 1, 7);
DECLARE_DOOR_TRIGGER(3, 11, 14);

void DoorTooltipAction(Event* evt);
void DoorKeyAction(Event* evt);

bool GlassesTipTrigger(Event* evt);
void GlassesTipAction(Event* evt);

#endif // EVENTS_H
