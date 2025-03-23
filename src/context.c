#include "context.h"
#include "map.h"
#include "ui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void ContextInit(Context* ctx)
{
    ctx->engine.running = true;
    ctx->settings.fov = 60;
    ctx->sdl.screen_width = DEFAULT_SCREEN_WIDTH;
    ctx->sdl.screen_height = DEFAULT_SCREEN_HEIGHT;
    ctx->raycaster.texture_width = 64;
    ctx->raycaster.texture_height = 64;
    ctx->settings.mouse_sensitivity = 0.25;
    ctx->settings.mouse_inverted = true;
    ctx->settings.render_distance = 20.0f;

    ctx->settings.enable_fog = true;
    ctx->settings.fog_distance = 10;
}

void ContextFree(Context* ctx)
{
    if (ctx) {
        PlayerFree(&ctx->level.player);
        FreeTextures(ctx);
        FreeSprites(ctx);
        FreeEntities(ctx);
        FreeItems(ctx);
        FreeEvents(ctx);
        UIClose(&ctx->ui);
        MapFree(ctx->level.map, ctx->level.map_height);
        CleanupThreads(ctx);

        if (ctx->sdl.renderer) {
            SDL_DestroyRenderer(ctx->sdl.renderer);
            ctx->sdl.renderer = NULL;
        }

        if (ctx->sdl.window) {
            SDL_DestroyWindow(ctx->sdl.window);
            ctx->sdl.window = NULL;
        }
    }
}

