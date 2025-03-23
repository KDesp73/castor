#define TARGET_FPS 60

#include "animation.h"
#include "cli.h"
#include "context.h"
#include "engine.h"
#include "game_player.h"
#include "ingame-ui.h"
#include "inventory.h"
#include "level.h"
#include "levels.h"
#include "movement.h"
#include "player.h"
#include "raycaster.h"
#include "screens.h"
#include "settings.h"
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

Uint8 HandleInput(Context* ctx, float deltaTime);
void HandleEvent(Context* ctx, SDL_Event* event, bool* paused);
void HandleLevelTransition(Context* ctx, SDL_Event* event);
void HandleKeyInput(Context* ctx, Uint8 key, Player* stored_player, float deltaTime);
void RenderFrame(Context* ctx, Animation* keyAnim);
void HandleLevelFail(Context* ctx, SDL_Event* event);

void loop(Context* ctx)
{
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

    // Start screen
    ctx->engine.running = false;
    if (UI_POLL_SCREEN(StartScreen, ctx, &event)) goto exit;
    ctx->engine.running = true;

    bool paused = false;

    while (ctx->engine.running) {
        FPS_START(ctx);

        while (SDL_PollEvent(&event)) {
            HandleEvent(ctx, &event, &paused);
        }

        if (ctx->level.next) {
            HandleLevelTransition(ctx, &event);
        } else if (ctx->level.fail) {
            HandleLevelFail(ctx, &event);
        } else if (!paused) {
            Uint8 key = HandleInput(ctx, deltaTime);
            HandleKeyInput(ctx, key, &stored_player, deltaTime);

            UpdateEntities(ctx, deltaTime);
            ProcessEvents(ctx);
            ItemsIdle(ctx, SDL_GetTicks() / 1000.0f);

            UpdateDamageNumbers(ctx);
            if(INV.key) UpdateAnimation(&keyAnim, SDL_GetTicks());

            EVERY_MS(soundCleanupTimer, 15000, {
                CleanupThreads(ctx);
            });

            RenderFrame(ctx, &keyAnim);
        }

        FPS_END(ctx);
    }

exit:
    FreeAnimation(&keyAnim);
}

static CliArgs args;

void setup(Context* ctx)
{
    ctx->game_name = "RayCasting";
    ctx->level.player = PlayerNew(10, 140, 0.0, 1.5, 1.5);
    ctx->raycaster.texture_width = 64;
    ctx->raycaster.texture_height = 64;
    ctx->raycaster.floor_texture_index = 9;
    ctx->raycaster.ceiling_texture_index = 7;
    ctx->settings.mouse_sensitivity = 0.3;
    ctx->settings.fullscreen = false;
    ctx->level.index = args.level;

    LoadLevel(ctx, Level(ctx->level.index));
    SetFullscreen(ctx, args.fullscreen);
}

int main(int argc, char** argv)
{
    args = ParseCliArgs(argc, argv);
    return EngineMain(argc, argv);
}

Uint8 HandleInput(Context* ctx, float deltaTime)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_ShowCursor(SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);

    if (xrel != 0) {
        RotateX(ctx, xrel * ctx->settings.mouse_sensitivity * 100, deltaTime);
    }

    if (yrel != 0) {
        RotateY(ctx, ((ctx->settings.mouse_inverted) ? -yrel : yrel) * (ctx->settings.mouse_sensitivity*100 + 0.15), deltaTime);
    }

    if (keys[SDL_SCANCODE_W]) {
        MoveFront(ctx, deltaTime);
    }
    if (keys[SDL_SCANCODE_S]) {
        MoveBack(ctx, deltaTime);
    }
    if (keys[SDL_SCANCODE_A]) {
        MoveLeft(ctx, deltaTime);
    }
    if (keys[SDL_SCANCODE_D]) {
        MoveRight(ctx, deltaTime);
    }

    if (keys[SDL_SCANCODE_UP]) {
        RotateY(ctx, ctx->level.player->angleDelta, deltaTime);
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        RotateY(ctx, -ctx->level.player->angleDelta * 2, deltaTime);
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        RotateX(ctx, -ctx->level.player->angleDelta, deltaTime);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        RotateX(ctx, ctx->level.player->angleDelta, deltaTime);
    }

    // For debugging
    for (size_t i = 4; i < 83; i++)
        if (keys[i]) return i;

    return 0;
}

void RenderFrame(Context* ctx, Animation* keyAnim)
{
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);

    CastWalls(ctx->sdl.renderer, ctx);
    CastSprites(ctx->sdl.renderer, ctx);
    RenderCrosshair(ctx->sdl.renderer, ctx->sdl.screen_width, ctx->sdl.screen_height);
    RenderHealthBar(ctx->sdl.renderer, 10, ctx->sdl.screen_height - 30, 100, 20, PLR.health, PLR.maxHealth);
    RenderDamageNumbers(ctx);
    UIRender(&ctx->ui, ctx);

    if (INV.key) {
        RenderAnimation(ctx->sdl.renderer, keyAnim, 10, 10, keyAnim->currentFrame);
    }

    SDL_RenderPresent(ctx->sdl.renderer);
}

// NOTE: Debugging method. Remove
void HandleKeyInput(Context* ctx, Uint8 key, Player* stored_player, float deltaTime)
{
    if (key == SDL_SCANCODE_R) {
        PlayerLoad(ctx->level.player, *stored_player);
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
    } else if(key == SDL_SCANCODE_K) {
        INV.key = true;
    }
}

void HandleEvent(Context* ctx, SDL_Event* event, bool* paused)
{
    if (event->type == SDL_QUIT) {
        ctx->engine.running = false;
    }

    if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        if (!(*paused)) {
            *paused = true;
            int result = UI_POLL_SCREEN(PauseScreen, ctx, event);

            if (result == 0) {
                *paused = false;

                // Clear ESCAPE key press before resuming
                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                        continue;
                    }
                    SDL_PushEvent(&e);
                }
            } else if (result == -1) {
                ctx->engine.running = false;
            }
        }
    }
}

void HandleLevelTransition(Context* ctx, SDL_Event* event)
{
    FreeLevel(ctx);
    ctx->level.index++;
    LoadLevel(ctx, Level(ctx->level.index));
    UI_POLL_SCREEN(LoadingScreen, ctx, event);
    ctx->level.next = false;
}

void HandleLevelFail(Context* ctx, SDL_Event* event)
{
    FreeLevel(ctx);
    LoadLevel(ctx, Level(1));

    UI_POLL_SCREEN(FailScreen, ctx, event);
    ctx->level.fail = false;
}
