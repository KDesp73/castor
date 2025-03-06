#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "raycaster.h"
#include "context.h"
#include "movement.h"
#include "textures.h"

void LoadTextures(Context* ctx)
{
    ctx->texture_width = 64;
    ctx->texture_height = 64;

    ctx->floor_texture = LoadTexture(ctx->renderer, "./assets/textures/stone.png");
    ctx->ceiling_texture = LoadTexture(ctx->renderer, "./assets/textures/wood.png");

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

    while (ctx->running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ctx->running = false;
            }
        }


        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W])
            MoveFront(ctx);
        if (keys[SDL_SCANCODE_S])
            MoveBack(ctx);
        if (keys[SDL_SCANCODE_A])
            RotateLeft(ctx);
        if (keys[SDL_SCANCODE_D])
            RotateRight(ctx);
        if (keys[SDL_SCANCODE_UP])
            LookUp(ctx);
        if (keys[SDL_SCANCODE_DOWN])
            LookDown(ctx);
        if(keys[SDL_SCANCODE_R])
            PlayerLoad(ctx->player, stored_player);

        SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
        SDL_RenderClear(ctx->renderer);


        // DrawFloorAndCeiling(ctx->renderer, ctx);
        CastRays(ctx->renderer, ctx);

        SDL_RenderPresent(ctx->renderer);
    }
    TTF_CloseFont(font);
}

int main(int argc, char** argv)
{
    Context ctx = {
        .game_name = "3d-game",
        .running = true,
        .map = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 6, 4, 3, 7, 2, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        },
        .screen_width = 1080,
        .screen_height = 720,
        .map_width = 16,
        .map_height = 16,
        .fov = 60,
        .player = PlayerNew(0.05, 0.0, 3.5, 3.5),
    };


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        ContextFree(&ctx);
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG))) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        ContextFree(&ctx);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    if (!ConstructRenderer(&ctx)) {
        fprintf(stderr, "Renderer initialization failed\n");
        ContextFree(&ctx);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    LoadTextures(&ctx);

    loop(&ctx);

    ContextFree(&ctx);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
