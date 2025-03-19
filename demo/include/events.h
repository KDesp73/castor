#ifndef EVENTS_H
#define EVENTS_H

#include "context.h"
#include "player.h"
#include <stdbool.h>

bool PlayerAttackTrigger(Event* evt);
void PlayerAttackAction(Event* evt);
bool PlayerIsAttacking(Player* player, Context* ctx);

bool trigger1(Event* evt);
void action1(Event* evt);


#endif // EVENTS_H
