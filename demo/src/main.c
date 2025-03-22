#include "game_player.h"
#define TARGET_FPS 60

#include "animation.h"
#include "context.h"
#include "engine.h"
#include "ingame-ui.h"
#include "inventory.h"
#include "level.h"
#include "levels.h"
#include "movement.h"
#include "player.h"
#include "raycaster.h"
#include "screens.h"
#include "ui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_clipboard.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

Uint8 HandleInput(Context* ctx, float elapsedTime);


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

        if(ctx->level.next) {
            printf("Loading level %d...\n", ctx->level.index);
            FreeLevel(ctx);
            ctx->level.index++;
            LoadLevel(ctx, Level(ctx->level.index));
            UI_POLL_SCREEN(LoadingScreen, ctx, &event);
            ctx->level.next = false;
        } else if (!paused) {
            Uint8 key = HandleInput(ctx, deltaTime);
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

            // CastFloorAndCeiling(ctx->sdl.renderer, ctx);
            CastWalls(ctx->sdl.renderer, ctx);
            CastSprites(ctx->sdl.renderer, ctx);
            RenderCrosshair(ctx->sdl.renderer, ctx->sdl.screen_width, ctx->sdl.screen_height);
            RenderHealthBar(ctx->sdl.renderer, 10, ctx->sdl.screen_height - 30, 100, 20, PLR.health, PLR.maxHealth);

            UpdateEntities(ctx, deltaTime);
            ProcessEvents(ctx);
            UIUpdate(&ctx->ui, ctx);
            ItemsIdle(ctx, SDL_GetTicks() / 1000.0f);

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
    ctx.level.player = PlayerNew(10, 140, 0.0, 1.5, 1.5);
    ctx.raycaster.texture_width = 64;
    ctx.raycaster.texture_height = 64;
    ctx.raycaster.floor_texture_index = 9;
    ctx.raycaster.ceiling_texture_index = 7;
    ctx.settings.mouse_sensitivity = 0.3;
    ctx.settings.fullscreen = false;
    ctx.level.index = 0;

    if(!EngineInit(&ctx)) {
        EngineClose(&ctx);
        return 1;
    }
    LoadTextures(&ctx);

    LoadLevel(&ctx, Level(ctx.level.index));
    // SetFullscreen(&ctx, true);

    loop(&ctx);

    printf("Closing...\n");
    EngineClose(&ctx);
    return 0;
}

Uint8 HandleInput(Context* ctx, float elapsedTime)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_ShowCursor(SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);

    if (xrel != 0) {
        RotateX(ctx, xrel * ctx->settings.mouse_sensitivity * 100, elapsedTime);
    }

    if (yrel != 0) {
        RotateY(ctx, ((ctx->settings.mouse_inverted) ? -yrel : yrel) * (ctx->settings.mouse_sensitivity*100 + 0.15), elapsedTime);
    }

    if (keys[SDL_SCANCODE_W]) {
        MoveFront(ctx, elapsedTime);
    }
    if (keys[SDL_SCANCODE_S]) {
        MoveBack(ctx, elapsedTime);
    }
    if (keys[SDL_SCANCODE_A]) {
        MoveLeft(ctx, elapsedTime);
    }
    if (keys[SDL_SCANCODE_D]) {
        MoveRight(ctx, elapsedTime);
    }

    if (keys[SDL_SCANCODE_UP]) {
        RotateY(ctx, ctx->level.player->angleDelta, elapsedTime);
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        RotateY(ctx, -ctx->level.player->angleDelta * 2, elapsedTime);
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        RotateX(ctx, -ctx->level.player->angleDelta, elapsedTime);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        RotateX(ctx, ctx->level.player->angleDelta, elapsedTime);
    }

    // For debugging
    for (size_t i = 4; i < 83; i++)
        if (keys[i]) return i;

    return 0;
}

