#ifndef EVENTS_H
#define EVENTS_H

#include "event.h"
#include <stdbool.h>

bool PlayerAttackTrigger(Event* evt);
void PlayerAttackAction(Event* evt);

bool trigger1(Event* evt);
void action1(Event* evt);

bool PickItemTrigger(Event* evt);
void PickItemAction(Event* evt);

#endif // EVENTS_H
