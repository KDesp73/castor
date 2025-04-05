#include "core.h"
#include "ui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void castor_ContextInit(castor_Context* ctx)
{
    ctx->engine.running = true;
    ctx->settings.fov = 60;
    ctx->sdl.screen_width = DEFAULT_SCREEN_WIDTH;
    ctx->sdl.screen_height = DEFAULT_SCREEN_HEIGHT;
    ctx->raycaster.texture_width = 64;
    ctx->raycaster.texture_height = 64;
    ctx->settings.mouse_inverted = true;
    ctx->settings.mouse_sensitivity = 0.25;
    ctx->settings.render_distance = 20.0f;

    ctx->settings.enable_fog = true;
    ctx->settings.fog_distance = 10;

    ctx->sound.volume = 50;
}

void castor_ContextFree(castor_Context* ctx)
{
    if (ctx) {
       castor_PlayerFree(&ctx->level.player);
       castor_FreeTextures(ctx);
       castor_FreeSprites(ctx);
       castor_FreeEntities(ctx);
       castor_FreeItems(ctx);
       castor_FreeEvents(ctx);
       UIClose(&ctx->ui);
       castor_MapFree(&ctx->level.map);
       castor_CleanupThreads(ctx);

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

