#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "raycaster.h"
#include "context.h"
#include "text.h"

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
            PlayerMoveFront(ctx);
        if (keys[SDL_SCANCODE_S])
            PlayerMoveBack(ctx);
        if (keys[SDL_SCANCODE_A])
            PlayerRotateLeft(ctx);
        if (keys[SDL_SCANCODE_D])
            PlayerRotateRight(ctx);
        if (keys[SDL_SCANCODE_UP])
            PlayerLookUp(ctx);
        if (keys[SDL_SCANCODE_DOWN])
            PlayerLookDown(ctx);
        if(keys[SDL_SCANCODE_R])
            PlayerLoad(ctx->player, stored_player);

        SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
        SDL_RenderClear(ctx->renderer);

        CastRays(ctx->renderer, ctx);

        SDL_RenderPresent(ctx->renderer);
    }
    TTF_CloseFont(font);
}

int main(int argc, char** argv)
{
    Context ctx = {.game_name = "3d-game",
        .running = true,
        .map = {
            {1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2},
            {2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {2, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1},
        },
        .screen_width = 1080,
        .screen_height = 720,
        .map_width = 16,
        .map_height = 16,
        .fov = 60,
        .player = PlayerNew(0.02, 0.0, 3.5, 3.5)};

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    ConstructRenderer(&ctx);

    loop(&ctx);
    printf("Closing...\n");

    ContextFree(&ctx);

    SDL_Quit();
    TTF_Quit();

    return 0;
}
