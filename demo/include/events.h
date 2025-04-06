#ifndef EVENTS_H
#define EVENTS_H

#include "core.h"
#include "ingame_ui.h"
#include "inventory.h"
#include <stdbool.h>

bool PlayerAttackTrigger(castor_Event* evt);
void PlayerAttackAction(castor_Event* evt);

bool EnemyAttackTrigger(castor_Event* evt);
void EnemyAttackAction(castor_Event* evt);

bool PickItemTrigger(castor_Event* evt);
void PickItemAction(castor_Event* evt);


bool GlitchTrigger(castor_Event* evt);
#define DECLARE_GLITCH_ACTION(num, code) \
    static inline void Glitch##num##Action(castor_Event* evt) \
    { \
        castor_Context* ctx = evt->ctx; \
        GlitchActivated = true; \
        code \
    }

DECLARE_GLITCH_ACTION(0, {
    ctx->level.map->grid[5][13] = 0;
    ctx->level.map->grid[13][9] = 0;
})

DECLARE_GLITCH_ACTION(1, {
    ctx->level.map->grid[17][11] = 0;
    ctx->level.map->grid[17][12] = 0;
})

DECLARE_GLITCH_ACTION(2, {
    ctx->level.map->grid[5][1] = 0;
    ctx->level.map->grid[6][1] = 0;
    ctx->level.map->grid[7][1] = 0;
    ctx->level.map->grid[8][1] = 0;
    ctx->level.map->grid[9][1] = 0;
    ctx->level.map->grid[10][1] = 0;
    ctx->level.map->grid[11][1] = 0;
    ctx->level.map->grid[5][2] = 0;
    ctx->level.map->grid[6][2] = 0;
    ctx->level.map->grid[7][2] = 0;
    ctx->level.map->grid[8][2] = 0;
    ctx->level.map->grid[9][2] = 0;
    ctx->level.map->grid[10][2] = 0;
    ctx->level.map->grid[11][2] = 0;
})

DECLARE_GLITCH_ACTION(4, {
    ctx->level.map->grid[2][2] = 0;
    ctx->level.map->grid[3][2] = 0;
    ctx->level.map->grid[27][2] = 0;
    ctx->level.map->grid[28][2] = 0;
})

DECLARE_GLITCH_ACTION(5, {
    ctx->level.map->grid[11][17] = 0;
    ctx->level.map->grid[11][18] = 0;
    ctx->level.map->grid[12][17] = 0;
    ctx->level.map->grid[12][18] = 0;
})

#define DECLARE_DOOR_TRIGGER(num, _x, _y)                      \
    static inline bool Door##num##Trigger(castor_Event* evt)          \
    {                                                          \
        castor_Context* ctx = (evt)->ctx;                             \
        castor_Player* player = ctx->level.player;                    \
        if (!(INV.key)) return false;                          \
        return (                                               \
            ((player->X) >= (_x) && (player->X) < (_x) + 1) && \
            ((player->Y) >= (_y) && (player->Y) < (_y) + 1)    \
        );                                                     \
    }


DECLARE_DOOR_TRIGGER(0, 7, 14);
DECLARE_DOOR_TRIGGER(1, 6, 3);
DECLARE_DOOR_TRIGGER(2, 1, 7);
DECLARE_DOOR_TRIGGER(3, 11, 14);
DECLARE_DOOR_TRIGGER(4, 4, 29);

void DoorTooltipAction(castor_Event* evt);
void DoorKeyAction(castor_Event* evt);

bool GlassesTipTrigger(castor_Event* evt);
void GlassesTipAction(castor_Event* evt);

bool TeleportTrigger(castor_Event* evt);
void TeleportAction(castor_Event* evt);

bool CreditsTrigger(castor_Event* evt);
void CreditsAction(castor_Event* evt);

bool EETrigger(castor_Event *evt);
void EEAction(castor_Event *evt);

#endif // EVENTS_H
