#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
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

#define TARGET_FPS 60
#define FRAME_DELAY (1000/TARGET_FPS)

void _LoadTextures(Context* ctx)
{
    ctx->textures[1] = LoadTexture(ctx->renderer, "./assets/textures/mossy-rock.png");
    ctx->textures[2] = LoadTexture(ctx->renderer, "./assets/textures/brick.png");
    ctx->textures[3] = LoadTexture(ctx->renderer, "./assets/textures/purple-rock.png");
    ctx->textures[4] = LoadTexture(ctx->renderer, "./assets/textures/eagle.png");
    ctx->textures[5] = LoadTexture(ctx->renderer, "./assets/textures/colored-rock.png");
    ctx->textures[6] = LoadTexture(ctx->renderer, "./assets/textures/blue-rock.png");
    ctx->textures[7] = LoadTexture(ctx->renderer, "./assets/textures/stone.png");
    ctx->textures[8] = LoadTexture(ctx->renderer, "./assets/textures/wood.png");

}

void loop(Context* ctx)
{
    if (!ctx || !ctx->player) {
        fprintf(stderr, "Error: Context or Player is NULL!\n");
        return;
    }

    Player stored_player = PlayerStore(ctx->player);

    SDL_Event event;
    TTF_Font *font = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 18);

    Uint32 frameStart, frameTime;
    while (ctx->running) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ctx->running = false;
            }
        }

        Uint8 key = HandleInput(ctx);
        if(key == SDL_SCANCODE_R)
            PlayerLoad(ctx->player, stored_player);

        SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
        SDL_RenderClear(ctx->renderer);


        // DrawFloorAndCeiling(ctx->renderer, ctx);
        CastRays(ctx->renderer, ctx);

        SDL_RenderPresent(ctx->renderer);

        frameTime = SDL_GetTicks() - frameStart;
        // Delay to cap FPS
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    TTF_CloseFont(font);
}

int main(int argc, char** argv)
{
    Context ctx = {
        .game_name = "3d-game",
        .player = PlayerNew(0.15, 0.0, 1.5, 1.5),
        .texture_width = 64,
        .texture_height = 64,
        .floor_texture_index = 7,
        .ceiling_texture_index = 8,
        .mouse_sensitivity = 0.4
    };
    ContextInit(&ctx);
    LoadLevel(&ctx, "levels/4.lvl");

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
