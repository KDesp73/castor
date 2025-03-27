#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct castor_Event{
    bool (*trigger)(struct castor_Event* self);
    void (*action)(struct castor_Event* self);
    void* ctx; // Reference to the global context
    void* state; // Reference to a user-defined context
    bool trigger_once;
    bool triggered;
    bool first_run;
    Uint32 cooldown;
    Uint32 last_processed;
    int index;
    char id[16];
} castor_Event;

typedef bool (*EventTrigger)(struct castor_Event* self);
typedef void (*EventAction)(struct castor_Event* self);


castor_Event* castor_EventNew(void* ctx, bool trigger_once, Uint32 cooldown, EventTrigger trigger, EventAction action);
void castor_EventFree(castor_Event** evt);
void castor_EventProcess(castor_Event* evt);

#endif // EVENT_H
