#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "engine.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "context.h"
#include "movement.h"
#include "settings.h"
#include "textures.h"
#include "ui.h"

#define TARGET_FPS 60

void loop(Context* ctx)
{
    if (!ctx || !ctx->player) {
        fprintf(stderr, "Error: Context or Player is NULL!\n");
        return;
    }

    Player stored_player = PlayerStore(ctx->player);

    SDL_Event event;

    static UIFont global = {0};
    UIFontOpen(&global, UI_GLOBAL_FONT, 20, UI_COLOR_WHITE);
    static UI ui = {0};
    UIOpen(&ui, ctx, &global);

    ctx->running = false;
    UI_POLL_SCREEN(StartScreen(ctx->renderer, &event, &ui), ctx, event);
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
                    UI_POLL_SCREEN(PauseScreen(ctx->renderer, &event, &ui), ctx, event);
                    paused = false; // Resume when PauseScreen() exits
                }
            }
        }

        if (!paused) {  // Only update game if not paused
            Uint8 key = HandleInput(ctx);
            if (key == SDL_SCANCODE_R) {
                PlayerLoad(ctx->player, stored_player);
            }

            SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
            SDL_RenderClear(ctx->renderer);

            CastRays(ctx->renderer, ctx);

            SDL_RenderPresent(ctx->renderer);
        }

        FPS_END(ctx);
    }
    UIClose(&ui);
}

int main(int argc, char** argv)
{
    Context ctx = {
        .game_name = "RayCasting",
        .player = PlayerNew(0.15, 0.0, 1.5, 1.5),
        .texture_width = 64,
        .texture_height = 64,
        .floor_texture_index = 7,
        .ceiling_texture_index = 8,
        .mouse_sensitivity = 0.4,
    };
    ContextInit(&ctx);
    LoadLevel(&ctx, "levels/2.lvl");

    if(!EngineInit(&ctx)) {
        EngineClose(&ctx);
        return 1;
    }
    // SetFullscreen(&ctx);

    LoadTextures(ctx.renderer, ctx.textures, "./textures.list");

    loop(&ctx);

    EngineClose(&ctx);

    return 0;
}
