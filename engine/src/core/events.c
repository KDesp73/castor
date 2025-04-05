#include "core.h"
#include <string.h>


void castor_AppendEvent(castor_Context* ctx, castor_Event* evt)
{
    if(ctx->level.event_count >= MAX_EVENTS) return;

    ctx->level.events[ctx->level.event_count++] = evt;
}


void castor_ProcessEvents(castor_Context* ctx)
{
    for(size_t i = 0; i < ctx->level.event_count; i++) {
        castor_Event* ev = ctx->level.events[i];
        if(!ev) continue;

        castor_EventProcess(ev);
    }
}

castor_Event* castor_SearchEvent(castor_Context* ctx, const char* id)
{
    for(size_t i = 0; i < ctx->level.event_count; i++) {
        castor_Event* ev = ctx->level.events[i];
        if(!ev) continue;
        if(strlen(ev->id) == 0) continue;

        if(!strcmp(ev->id, id)) return ev;
    }

    return NULL;
}

void castor_FreeEvents(castor_Context* ctx)
{
    for(size_t i = 0; i < ctx->level.event_count; i++){
        if(ctx->level.events[i]){
            castor_EventFree(&ctx->level.events[i]);
        }
    }
    ctx->level.event_count = 0;
}
