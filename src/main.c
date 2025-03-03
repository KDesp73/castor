#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "raycaster.h"
#include "context.h"

void loop(Context* ctx)
{
    SDL_Event event;

    while (ctx->running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                ctx->running = false;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W]) {
            ctx->player->X += cos(ctx->player->angle * M_PI / 180.0) * ctx->player->speed;
            ctx->player->Y += sin(ctx->player->angle * M_PI / 180.0) * ctx->player->speed;
        }
        if (keys[SDL_SCANCODE_S]) {
            ctx->player->X -= cos(ctx->player->angle * M_PI / 180.0) * ctx->player->speed;
            ctx->player->Y -= sin(ctx->player->angle * M_PI / 180.0) * ctx->player->speed;
        }
        if (keys[SDL_SCANCODE_A]) {
            ctx->player->angle -= 1;
        }
        if (keys[SDL_SCANCODE_D]) {
            ctx->player->angle += 1;
        }

        SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
        SDL_RenderClear(ctx->renderer);

        CastRays(ctx->renderer, ctx);

        SDL_RenderPresent(ctx->renderer);
    }
}

int main(int argc, char** argv)
{
    Context ctx = {.game_name = "3d-game",
        .running = true,
        .map = {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 1},
            {1, 0, 1, 0, 0, 1, 0, 1},
            {1, 0, 1, 0, 0, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1}
        },
        .screen_width = 800,
        .screen_height = 600,
        .map_width = 8,
        .map_height = 8,
        .fov = 60,
        .player = PlayerNew(0.02, 0.0, 3.5, 3.5)};

    SDL_Init(SDL_INIT_VIDEO);
    ConstructRenderer(&ctx);

    loop(&ctx);

    ContextFree(&ctx);

    SDL_Quit();

    return 0;
}
