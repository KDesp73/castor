#include <SDL2/SDL.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "animation.h"
#include "engine.h"
#include "ingame-ui.h"
#include "level.h"
#include "levels.h"
#include "player.h"
#include "raycaster.h"
#include "context.h"
#include "movement.h"
#include "ui.h"
#include "screens.h"
#include "inventory.h"

#define TARGET_FPS 60


void loop(Context* ctx) {
    if (!ctx || !ctx->level.player) {
        fprintf(stderr, "Error: Context or Player is NULL!\n");
        return;
    }

    Player stored_player = PlayerStore(ctx->level.player);

    SDL_Event event;

    static UIFont global = {0};
    UIFontOpen(&global, UI_GLOBAL_FONT, 18, UI_COLOR_WHITE);
    UIOpen(&ctx->ui, &global);

    Animation keyAnim = LoadAnimation(ctx->sdl.renderer, "assets/animations/key.png", 32, 32, 50);

    ctx->engine.running = false;
    if (UI_POLL_SCREEN(StartScreen, ctx, &event)) goto exit;
    ctx->engine.running = true;

    bool paused = false;
    while (ctx->engine.running) {
        FPS_START(ctx);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ctx->engine.running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                if (!paused) {
                    paused = true;
                    int result = UI_POLL_SCREEN(PauseScreen, ctx, &event);
                    if (result == 0) {
                        paused = false;
                        SDL_Event e;
                        while (SDL_PollEvent(&e)) {
                            if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                                continue;
                            }
                            SDL_PushEvent(&e);
                        }
                    } else if (result == -1) {
                        goto exit;
                    }
                }
            }
        }

        if (!paused) {
            Uint8 key = HandleInput(ctx);
            if (key == SDL_SCANCODE_R) {
                PlayerLoad(ctx->level.player, stored_player);
            } else if (key == SDL_SCANCODE_T) {
                if (ctx->raycaster.textures_loaded) {
                    FreeTextures(ctx);
                } else {
                    LoadTextures(ctx);
                }
            } else if (key == SDL_SCANCODE_C) {
                char buffer[64];
                snprintf(buffer, 64, "(%.0f, %.0f)", ctx->level.player->X, ctx->level.player->Y);
                printf("%s\n", buffer);
                SDL_SetClipboardText(buffer);
            }

            SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
            SDL_RenderClear(ctx->sdl.renderer);

            CastWalls(ctx->sdl.renderer, ctx);
            CastSprites(ctx->sdl.renderer, ctx);
            RenderCrosshair(ctx->sdl.renderer, ctx->sdl.screen_width, ctx->sdl.screen_height);

            UpdateEntities(ctx, ctx->engine.frame_time / 1000.0f);
            ProcessEvents(ctx);
            UIUpdate(&ctx->ui, ctx);
            ItemsIdle(ctx,  SDL_GetTicks() / 1000.0f);

            UpdateDamageNumbers(ctx);
            RenderDamageNumbers(ctx);

            EVERY_MS(soundCleanupTimer, 15000, {
                CleanupThreads(ctx);
            });

            if (INV.key) {
                UpdateAnimation(&keyAnim, SDL_GetTicks());
                RenderAnimation(ctx->sdl.renderer, &keyAnim, 10, 10, keyAnim.currentFrame);
            }

            SDL_RenderPresent(ctx->sdl.renderer);
        }

        FPS_END(ctx);
    }

exit:
    FreeAnimation(&keyAnim);
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    Context ctx = {0};
    ContextInit(&ctx);
    ctx.game_name = "RayCasting";
    ctx.level.player = PlayerNew(0.15, 0.0, 1.5, 1.5);
    ctx.raycaster.texture_width = 64;
    ctx.raycaster.texture_height = 64;
    ctx.raycaster.floor_texture_index = 6;
    ctx.raycaster.ceiling_texture_index = 7;
    ctx.settings.mouse_sensitivity = 0.3;
    ctx.settings.fullscreen = false;

    if(!EngineInit(&ctx)) {
        EngineClose(&ctx);
        return 1;
    }
    LoadTextures(&ctx);

    LoadLevel(&ctx, Level2);
    // SetFullscreen(&ctx, true);

    loop(&ctx);

    printf("Closing...\n");
    EngineClose(&ctx);
    return 0;
}
