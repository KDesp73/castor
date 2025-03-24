#include "context.h"
#include <string.h>


void AppendEvent(Context* ctx, Event* evt)
{
    if(ctx->level.event_count >= MAX_EVENTS) return;

    ctx->level.events[ctx->level.event_count++] = evt;
}


void ProcessEvents(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.event_count; i++) {
        Event* ev = ctx->level.events[i];
        if(!ev) continue;

        EventProcess(ev);
    }
}

Event* SearchEvent(Context* ctx, const char* id)
{
    for(size_t i = 0; i < ctx->level.event_count; i++) {
        Event* ev = ctx->level.events[i];
        if(!ev) continue;
        if(strlen(ev->id) == 0) continue;

        if(!strcmp(ev->id, id)) return ev;
    }

    return NULL;
}

void FreeEvents(Context* ctx)
{
    for(size_t i = 0; i < ctx->level.event_count; i++){
        if(ctx->level.events[i]){
            EventFree(&ctx->level.events[i]);
        }
    }
    ctx->level.event_count = 0;
}
