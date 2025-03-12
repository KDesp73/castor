#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include "animation.h"
#include "engine.h"
#include "level.h"
#include "levels.h"
#include "player.h"
#include "raycaster.h"
#include "context.h"
#include "movement.h"
#include "textures.h"
#include "ui.h"
#include "screens.h"

#define TARGET_FPS 60

static struct {
    bool keyAquired;
    size_t potionsCount;
} Inventory;

static void InventoryInit()
{
    Inventory.keyAquired = false;
    Inventory.potionsCount = 1;
}

void loop(Context* ctx)
{
    if (!ctx || !ctx->player) {
        fprintf(stderr, "Error: Context or Player is NULL!\n");
        return;
    }

    Player stored_player = PlayerStore(ctx->player);

    SDL_Event event;

    static UIFont global = {0};
    UIFontOpen(&global, UI_GLOBAL_FONT, 18, UI_COLOR_WHITE);
    static UI ui = {0};
    UIOpen(&ui, ctx, &global);

    UIToast toast = {0};
    UIToastInit(&toast, "Hello World!", 5000);

    Animation keyAnim = LoadAnimation(ctx->renderer, "assets/animations/key.png", 32, 32, 50);

    ctx->running = false;
    if(UI_POLL_SCREEN(StartScreen, ctx->renderer, &event, &ui)) goto exit;
    ctx->running = true;

    bool paused = false;
    while (ctx->running) {
        FPS_START(ctx);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ctx->running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                if (!paused) {
                    paused = true;
                    int result = UI_POLL_SCREEN(PauseScreen, ctx->renderer, &event, &ui);
                    if (result == 0) paused = false;
                    else if(result == -1) goto exit;
                }
            }
        }

        if (!paused) {
            Uint8 key = HandleInput(ctx);
            if (key == SDL_SCANCODE_R) {
                PlayerLoad(ctx->player, stored_player);
            } else if(key == SDL_SCANCODE_T) {
                if(ctx->textures_loaded) {
                    FreeTextures(ctx);
                } else {
                    LoadTextures(ctx);
                }
            } else if(key == SDL_SCANCODE_K)
                Inventory.keyAquired = !Inventory.keyAquired;

            SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
            SDL_RenderClear(ctx->renderer);

            // CastFloorAndCeiling(ctx->renderer, ctx);
            CastWalls(ctx->renderer, ctx);
            CastSprites(ctx->renderer, ctx);
            UIToastRender(ctx->renderer, &global, &toast, ctx->screen_width, ctx->screen_height);

            if(Inventory.keyAquired) {
                UpdateAnimation(&keyAnim, SDL_GetTicks());
                RenderAnimation(ctx->renderer, &keyAnim, 10, 10, keyAnim.currentFrame);
            }

            SDL_RenderPresent(ctx->renderer);
        }

        FPS_END(ctx);
    }

exit:
    FreeAnimation(&keyAnim);
    UIClose(&ui);
}

int main(int argc, char** argv)
{
    InventoryInit();

    Context ctx = {0};
    ContextInit(&ctx);
    ctx.game_name = "RayCasting";
    ctx.player = PlayerNew(0.15, 0.0, 1.5, 1.5);
    ctx.texture_width = 64;
    ctx.texture_height = 64;
    ctx.floor_texture_index = 6;
    ctx.ceiling_texture_index = 7;
    ctx.mouse_sensitivity = 0.3;
    ctx.fullscreen = false;

    if(!EngineInit(&ctx)) {
        EngineClose(&ctx);
        return 1;
    }
    LoadLevel(&ctx, Level2);

    LoadTextures(&ctx);

    loop(&ctx);

    printf("Closing...\n");
    EngineClose(&ctx);
    return 0;
}
