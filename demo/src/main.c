#define TARGET_FPS 60

#include "animation.h"
#include "cli.h"
#include "context.h"
#include "engine.h"
#include "game_player.h"
#include "image.h"
#include "ingame-ui.h"
#include "inventory.h"
#include "level.h"
#include "levels.h"
#include "movement.h"
#include "paths.h"
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
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

Uint8 HandleInput(Context* ctx, float deltaTime);
void DEBUG_HandleKeyInput(Context* ctx, Uint8 key, float deltaTime);
void HandleEvent(Context* ctx, SDL_Event* event, bool* paused);
void HandleLevelFail(Context* ctx, SDL_Event* event);
void HandleLevelTransition(Context* ctx, SDL_Event* event);
void RenderFrame(Context* ctx);

static Animation keyAnim;
static Image glassesImg;
static Image swordImg;
static Image invSquareImg;


static CliArgs args;

void presetup(Context* ctx)
{
    ctx->game_name = "RayCasting";
}

void setup(Context* ctx)
{
    ctx->level.player = PlayerNew(10, 140, 0.0, 1.5, 1.5);
    ctx->raycaster.texture_width = 64;
    ctx->raycaster.texture_height = 64;
    ctx->raycaster.floor_texture_index = 9;
    ctx->raycaster.ceiling_texture_index = 7;
    ctx->settings.mouse_sensitivity = 0.3;
    ctx->settings.fullscreen = false;
    ctx->level.index = args.level;
    ctx->sound.volume = 70;

    LoadLevel(ctx, Level(ctx->level.index));
    SetFullscreen(ctx, args.fullscreen);
    LoadTextures(ctx);

    static UIFont global = {0};
    UIFontOpen(&global, UI_GLOBAL_FONT, 18, UI_COLOR_WHITE);
    UIOpen(&ctx->ui, &global);

    keyAnim      = LoadAnimation(ctx->sdl.renderer, "assets/animations/key.png", 32, 32, 50);
    glassesImg   = LoadImage(ctx->sdl.renderer, "assets/sprites/glasses.png");
    swordImg     = LoadImage(ctx->sdl.renderer, "./assets/sprites/sword.png");
    invSquareImg = LoadImage(ctx->sdl.renderer, "./assets/sprites/inventory-square.png");
}

static bool started = false;
static bool paused = false;

void loop(Context* ctx)
{
    FPS_START(ctx);

    SDL_Event event;
    if(!started) {
        if (UI_POLL_SCREEN(StartScreen, ctx, &event)) ctx->engine.running = false;
        started = true;
    }

    while (SDL_PollEvent(&event)) {
        HandleEvent(ctx, &event, &paused);
    }

    if (ctx->level.next) {
        HandleLevelTransition(ctx, &event);
    } else if (ctx->level.fail) {
        HandleLevelFail(ctx, &event);
    } else if (!paused) {
        Uint8 key = HandleInput(ctx, deltaTime);
        DEBUG_HandleKeyInput(ctx, key, deltaTime);

        UpdateEntities(ctx, deltaTime);
        ProcessEvents(ctx);
        ItemsIdle(ctx, SDL_GetTicks() / 1000.0f);

        UpdateDamageNumbers(ctx);
        if(INV.key) UpdateAnimation(&keyAnim, SDL_GetTicks());

        EVERY_MS(soundCleanupTimer, 15000, {
            CleanupThreads(ctx);
        });

        RenderFrame(ctx);
    }

    FPS_END(ctx);
}

void cleanup(Context* ctx)
{
    FreeAnimation(&keyAnim);
    FreeImage(&glassesImg);
    FreeImage(&swordImg);
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
        RotateX(ctx, xrel * ctx->settings.mouse_sensitivity * 50, deltaTime);
    }

    if (yrel != 0) {
        RotateY(ctx, ((ctx->settings.mouse_inverted) ? -yrel : yrel) * (ctx->settings.mouse_sensitivity*50 + 0.15), deltaTime);
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

void RenderFrame(Context* ctx)
{
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);

    CastWalls(ctx->sdl.renderer, ctx);
    CastSprites(ctx->sdl.renderer, ctx);
    
    if(INV.sword) {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        bool attacking = keys[SDL_SCANCODE_SPACE] || (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

        if(!attacking)
            RenderRotatedImage(ctx->sdl.renderer, &swordImg, 20, ctx->sdl.screen_width / 2 - 200, ctx->sdl.screen_height - 350, 0);
        else
            RenderRotatedImage(ctx->sdl.renderer, &swordImg, 20, ctx->sdl.screen_width / 2 - 300, ctx->sdl.screen_height - 150, -35);
    }

    RenderCrosshair(ctx->sdl.renderer, ctx->sdl.screen_width, ctx->sdl.screen_height);
    RenderHealthBar(ctx->sdl.renderer, 10, ctx->sdl.screen_height - 45, 150, 30, PLR.health, PLR.maxHealth);
    RenderDamageNumbers(ctx);
    UIRender(&ctx->ui, ctx);

    float padding = 20.0f;
    int keyX = ctx->sdl.screen_width - 1*(2*keyAnim.frameWidth+padding);
    int keyY = ctx->sdl.screen_height - 2*keyAnim.frameHeight - padding;
    int swordX = ctx->sdl.screen_width - 2*(2*swordImg.w+padding);
    int swordY = ctx->sdl.screen_height - 2*swordImg.h - padding;
    int glassesX = ctx->sdl.screen_width - 3*(2*glassesImg.w+padding);
    int glassesY = ctx->sdl.screen_height - 2*glassesImg.w - padding;

    RenderImage(ctx->sdl.renderer, &invSquareImg, 1, keyX-8, keyY-8);
    RenderImage(ctx->sdl.renderer, &invSquareImg, 1, swordX-8, swordY-8);
    RenderImage(ctx->sdl.renderer, &invSquareImg, 1, glassesX-8, glassesY-8);

    if (INV.key) {
        RenderAnimation(ctx->sdl.renderer, &keyAnim, 2,
                keyX, keyY, keyAnim.currentFrame);
    }
    if(INV.sword) {
        RenderImage(ctx->sdl.renderer, &swordImg, 2,
                swordX, swordY);
    }
    if(INV.glasses) {
        RenderImage(ctx->sdl.renderer, &glassesImg, 2,
                glassesX, glassesY);
    }

    SDL_RenderPresent(ctx->sdl.renderer);
}

// NOTE: Debugging method. Remove
void DEBUG_HandleKeyInput(Context* ctx, Uint8 key, float deltaTime)
{
    if (key == SDL_SCANCODE_C) {
        char buffer[64];
        snprintf(buffer, 64, "(%.0f, %.0f)", ctx->level.player->X, ctx->level.player->Y);
        printf("%s\n", buffer);
        SDL_SetClipboardText(buffer);
    } else if(key == SDL_SCANCODE_K) {
        INV.key = true;
    } else if(key == SDL_SCANCODE_N) {
        ctx->level.next = true;
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
    if(UI_POLL_SCREEN(LoadingScreen, ctx, event) == -1) 
        ctx->engine.running = false;
    ctx->level.next = false;
}

void HandleLevelFail(Context* ctx, SDL_Event* event)
{
    FreeLevel(ctx);
    LoadLevel(ctx, Level(1));

    if(UI_POLL_SCREEN(FailScreen, ctx, event))
        ctx->engine.running = false;
    ctx->level.fail = false;
}
