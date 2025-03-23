#include "context.h"


void AppendEvent(Context* ctx, Event* evt)
{
    if(ctx->level.event_count >= MAX_EVENTS) return;

    ctx->level.events[ctx->level.event_count++] = evt;
}


void ProcessEvents(Context* ctx)
{
    Uint32 now = SDL_GetTicks();

    for(size_t i = 0; i < ctx->level.event_count; i++) {
        Event* ev = ctx->level.events[i];
        if(!ev) continue;

        if (now - ev->last_processed >= ev->cooldown) {
            EventProcess(ev);
            ev->last_processed = now;
        }
    }
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
