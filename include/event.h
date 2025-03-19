#ifndef EVENT_H
#define EVENT_H

#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Event{
    bool (*trigger)(struct Event* self);
    void (*action)(struct Event* self);
    void* ctx;
    bool trigger_once;
    bool triggered;
    Uint32 cooldown;
    Uint32 last_processed;
} Event;

typedef bool (*EventTrigger)(struct Event* self);
typedef void (*EventAction)(struct Event* self);


Event* EventNew(void* ctx, bool trigger_once, Uint32 cooldown, EventTrigger trigger, EventAction action);
void EventFree(Event** evt);
void EventProcess(Event* evt);

#endif // EVENT_H
