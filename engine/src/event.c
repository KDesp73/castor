#include "event.h"
#include <SDL2/SDL_timer.h>
#include <stdlib.h>

castor_Event* castor_EventNew(void* ctx, bool trigger_once, Uint32 cooldown, EventTrigger trigger, EventAction action)
{
    castor_Event* evt = malloc(sizeof(castor_Event));

    evt->ctx = ctx;
    evt->trigger = trigger;
    evt->action = action;
    evt->trigger_once = trigger_once;
    evt->triggered = false;
    evt->cooldown = cooldown;
    evt->first_run = true;

    return evt;
}

void castor_EventFree(castor_Event** evt)
{
    if(*evt) {
        free(*evt);
        *evt = NULL;
    }
}

void castor_EventProcess(castor_Event* evt)
{
    Uint32 now = SDL_GetTicks();

    if (evt->trigger_once && evt->triggered) return;

    if (evt->first_run || now - evt->last_processed >= evt->cooldown) {
        if (evt->trigger(evt)) {
            evt->action(evt);
            evt->triggered = true;
            evt->first_run = false;
            evt->last_processed = now;
        }
    }
}
