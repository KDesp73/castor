#include "event.h"
#include <stdlib.h>

Event* EventNew(void* ctx, bool trigger_once, EventTrigger trigger, EventAction action)
{
    Event* evt = malloc(sizeof(Event));

    evt->ctx = ctx;
    evt->trigger = trigger;
    evt->action = action;
    evt->trigger_once = trigger_once;

    return evt;
}

void EventFree(Event** evt)
{
    if(*evt) {
        free(*evt);
        *evt = NULL;
    }
}

void EventProcess(Event* evt)
{
    if(evt->trigger(evt)) {
        evt->action(evt);
    }
}

