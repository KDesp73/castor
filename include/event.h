#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Event{
    bool (*trigger)(struct Event* self);
    void (*action)(struct Event* self);
    void* ctx;
    bool trigger_once;
} Event;

typedef bool (*EventTrigger)(struct Event* self);
typedef void (*EventAction)(struct Event* self);


Event* EventNew(void* ctx, bool trigger_once, EventTrigger trigger, EventAction action);
void EventFree(Event** evt);
void EventProcess(Event* evt);

#endif // EVENT_H
