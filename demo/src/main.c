#define TARGET_FPS 60

#include "asset.h"
#include "cli.h"
#include "core.h"
#include "game_player.h"
#include "ingame_ui.h"
#include "inventory.h"
#include "levels.h"
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
#include <string.h>
#include <time.h>

void HandleInput(castor_Context* ctx, float deltaTime);
void DEBUG_HandleKeyInput(castor_Context* ctx, float deltaTime);
void HandleEvent(castor_Context* ctx, SDL_Event* event, bool* paused);
void HandleLevelFail(castor_Context* ctx, SDL_Event* event);
void HandleLevelTransition(castor_Context* ctx, SDL_Event* event);
void RenderFrame(castor_Context* ctx);

static castor_Animation keyAnim;
static castor_Image glassesImg;
static castor_Image swordImg;
static castor_Image invSquareImg;

static castor_Map* mapStored;

static CliArgs args;
bool GlitchActivated = false;
Particle particles[NUM_PARTICLES];

void presetup(castor_Context* ctx)
{
    ctx->game_name = "Eidolon";
}

void setup(castor_Context* ctx)
{
    ctx->level.player = castor_PlayerNew(5, 140);
    ctx->raycaster.texture_width = 64;
    ctx->raycaster.texture_height = 64;
    ctx->raycaster.floor_texture_index = 30;
    ctx->raycaster.ceiling_texture_index = 29;
    ctx->settings.fullscreen = false;
    ctx->settings.mouse_sensitivity = 0.35;
    ctx->settings.render_distance = 40.0f;
    ctx->level.index = 0;
    ctx->sound.volume = 100;

    if(args.level){
        castor_LoadLevelMap(ctx, args.level);
    } else {
        castor_LoadLevel(ctx, Level(ctx->level.index));
        mapStored = castor_MapCreate(ctx->level.map->h, ctx->level.map->w);
        castor_MapCpy(ctx->level.map, mapStored);
    }

    castor_SetFullscreen(ctx, args.fullscreen);
    castor_LoadTextures(ctx);
    castor_LoadSprites(ctx);

    static UIFont global = {0};
    UIFontOpen(&global, UI_GLOBAL_FONT, 18, UI_COLOR_WHITE);
    UIOpen(&ctx->ui, &global);

    keyAnim      = castor_LoadAnimation(ctx->sdl.renderer, "assets/animations/key.png", 32, 32, 50);
    glassesImg   = castor_LoadImage(ctx->sdl.renderer, "assets/sprites/glasses.png");
    swordImg     = castor_LoadImage(ctx->sdl.renderer, "./assets/sprites/sword.png");
    invSquareImg = castor_LoadImage(ctx->sdl.renderer, "./assets/sprites/inventory-square.png");

    initParticles(ctx);
}

static bool started = false;
static bool paused = false;

void loop(castor_Context* ctx)
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
        HandleInput(ctx, deltaTime);
        if(args.debug)
            DEBUG_HandleKeyInput(ctx, deltaTime);

        castor_UpdateEntities(ctx, deltaTime);
        castor_ProcessEvents(ctx);
        castor_ItemsIdle(ctx, SDL_GetTicks() / 1000.0f);

        UpdateDamageNumbers(ctx);
        if(INV.key) castor_UpdateAnimation(&keyAnim, SDL_GetTicks());

        if(GlitchActivated) {
            castor_FreeTextures(ctx);
        } else {
            if(!ctx->raycaster.textures_loaded)
                castor_LoadTextures(ctx);
        }

        if(ctx->level.index == 5) updateParticles(ctx);

        EVERY_MS(soundCleanupTimer, 15000, {
            castor_CleanupThreads(ctx);
            for(size_t i = 0; i < ctx->ui.toast_count; i++);
        });

        RenderFrame(ctx);
    }

    FPS_END(ctx);
}

void cleanup(castor_Context* ctx)
{
    castor_FreeAnimation(&keyAnim);
    castor_FreeImage(&glassesImg);
    castor_FreeImage(&swordImg);
}

int main(int argc, char** argv)
{
    args = ParseCliArgs(argc, argv);

    return castor_Main(argc, argv);
}

void HandleInput(castor_Context* ctx, float deltaTime)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_ShowCursor(SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    int xrel, yrel;
    SDL_GetRelativeMouseState(&xrel, &yrel);

#define BASE_DELTA 15
    if (xrel != 0) {
        castor_RotateX(ctx, xrel * ctx->settings.mouse_sensitivity * BASE_DELTA, deltaTime);
    }

    if (yrel != 0) {
        castor_RotateY(ctx, ((ctx->settings.mouse_inverted) ? -yrel : yrel) * (ctx->settings.mouse_sensitivity * BASE_DELTA), deltaTime);
    }

    float moveX = 0.0f;
    float moveY = 0.0f;

    if (keys[SDL_SCANCODE_W]) moveY += 1.0f;
    if (keys[SDL_SCANCODE_S]) moveY -= 1.0f;
    if (keys[SDL_SCANCODE_A]) moveX -= 1.0f;
    if (keys[SDL_SCANCODE_D]) moveX += 1.0f;

    float magnitude = sqrtf(moveX * moveX + moveY * moveY);
    if (magnitude > 0.0f) {
        moveX /= magnitude;
        moveY /= magnitude;
    }

    float speedMultiplier = (keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT]) ? 1.5f : 1.0f;

    if (moveY > 0) castor_MoveFront(ctx, deltaTime * moveY * speedMultiplier);
    if (moveY < 0) castor_MoveBack(ctx, deltaTime * -moveY * speedMultiplier);
    if (moveX > 0) castor_MoveRight(ctx, deltaTime * moveX * speedMultiplier);
    if (moveX < 0) castor_MoveLeft(ctx, deltaTime * -moveX * speedMultiplier);

    if (keys[SDL_SCANCODE_UP]) {
        castor_RotateY(ctx, ctx->level.player->angleDelta, deltaTime);
    }
    if (keys[SDL_SCANCODE_DOWN]) {
        castor_RotateY(ctx, -ctx->level.player->angleDelta * 2, deltaTime);
    }
    if (keys[SDL_SCANCODE_LEFT]) {
        castor_RotateX(ctx, -ctx->level.player->angleDelta, deltaTime);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        castor_RotateX(ctx, ctx->level.player->angleDelta, deltaTime);
    }
}

void RenderFrame(castor_Context* ctx)
{
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);

    castor_CastFloorAndCeiling(ctx->sdl.renderer, ctx);
    castor_CastWalls(ctx->sdl.renderer, ctx);
    castor_CastSprites(ctx->sdl.renderer, ctx);
    
    if(INV.sword) {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        bool attacking = keys[SDL_SCANCODE_SPACE] || (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

        if(!attacking)
            castor_RenderRotatedImage(ctx->sdl.renderer, &swordImg, 20, ctx->sdl.screen_width / 2 - 200, ctx->sdl.screen_height - 350, 0);
        else
            castor_RenderRotatedImage(ctx->sdl.renderer, &swordImg, 20, ctx->sdl.screen_width / 2 - 300, ctx->sdl.screen_height - 150, -35);
    }

    RenderCrosshair(ctx->sdl.renderer, ctx->sdl.screen_width, ctx->sdl.screen_height);
    RenderHealthBar(ctx->sdl.renderer, 10, ctx->sdl.screen_height - 45, 150, 30, PLR.health, PLR.maxHealth);
    
    castor_Event* glitch = castor_SearchEvent(ctx, "glitch");
    if(INV.glasses && glitch) {
        RenderGlassesCooldown(ctx->sdl.renderer, 10, ctx->sdl.screen_height - 85, 150, 30, SDL_GetTicks() - glitch->last_processed, glitch->cooldown);
        if(SDL_GetTicks() - glitch->last_processed >= 10000) {
            castor_MapCpy(mapStored, ctx->level.map);

            GlitchActivated = false;
        }
    }
    RenderDamageNumbers(ctx);
    UIRender(&ctx->ui, ctx);

    float padding = 20.0f;
    int keyX = ctx->sdl.screen_width - 1*(2*keyAnim.frameWidth+padding);
    int keyY = ctx->sdl.screen_height - 2*keyAnim.frameHeight - padding;
    int swordX = ctx->sdl.screen_width - 2*(2*swordImg.w+padding);
    int swordY = ctx->sdl.screen_height - 2*swordImg.h - padding;
    int glassesX = ctx->sdl.screen_width - 3*(2*glassesImg.w+padding);
    int glassesY = ctx->sdl.screen_height - 2*glassesImg.w - padding;

    castor_RenderImage(ctx->sdl.renderer, &invSquareImg, 1, keyX-8, keyY-8);
    castor_RenderImage(ctx->sdl.renderer, &invSquareImg, 1, swordX-8, swordY-8);
    castor_RenderImage(ctx->sdl.renderer, &invSquareImg, 1, glassesX-8, glassesY-8);

    if (INV.key) {
        castor_RenderAnimation(ctx->sdl.renderer, &keyAnim, 2,
                keyX, keyY, keyAnim.currentFrame);
    }
    if(INV.sword) {
        castor_RenderImage(ctx->sdl.renderer, &swordImg, 2,
                swordX, swordY);
    }
    if(INV.glasses) {
        castor_RenderImage(ctx->sdl.renderer, &glassesImg, 2,
                glassesX, glassesY);
    }

    if(ctx->level.index == 5) renderParticles(ctx->sdl.renderer);

    SDL_RenderPresent(ctx->sdl.renderer);
}

// NOTE: Debugging method.
void DEBUG_HandleKeyInput(castor_Context* ctx, float deltaTime)
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_C]) {
        char buffer[64];
        snprintf(buffer, 64, "(%.0f, %.0f)", ctx->level.player->X - 1, ctx->level.player->Y - 1);
        printf("%s\n", buffer);
        SDL_SetClipboardText(buffer);
    }
    if (keys[SDL_SCANCODE_K]) {
        INV.key = true;
    }
    if (keys[SDL_SCANCODE_N]) {
        ctx->level.next = true;
    }
}

void HandleEvent(castor_Context* ctx, SDL_Event* event, bool* paused)
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

void HandleLevelTransition(castor_Context* ctx, SDL_Event* event)
{
    castor_MapFree(&mapStored);

    castor_LevelLoader level = Level(ctx->level.index+1);
    if(!level) {
        ctx->engine.running = false;
        return;
    }
    castor_FreeLevel(ctx);
    ctx->level.index++;
    castor_LoadLevel(ctx, level);

    mapStored = castor_MapCreate(ctx->level.map->h, ctx->level.map->w);
    castor_MapCpy(ctx->level.map, mapStored);


    if(UI_POLL_SCREEN(LoadingScreen, ctx, event) == -1) 
        ctx->engine.running = false;
    ctx->level.next = false;
    GlitchActivated = false;
}

void HandleLevelFail(castor_Context* ctx, SDL_Event* event)
{
    castor_MapFree(&mapStored);

    castor_FreeLevel(ctx);
    castor_LoadLevel(ctx, Level(0));
    ctx->level.index = 0;

    mapStored = castor_MapCreate(ctx->level.map->h, ctx->level.map->h);
    castor_MapCpy(ctx->level.map, mapStored);

    if(UI_POLL_SCREEN(FailScreen, ctx, event))
        ctx->engine.running = false;
    ctx->level.fail = false;
}
