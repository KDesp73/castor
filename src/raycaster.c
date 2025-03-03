#include "raycaster.h"
#include "context.h"
#include <SDL2/SDL_render.h>

#include <stdbool.h>

void CastRays(SDL_Renderer *renderer, const Context* ctx)
{
    for (int x = 0; x < ctx->screen_width; x++) {
        float rayAngle = (ctx->player->angle - ctx->fov/ 2.0) + ((float)x / ctx->screen_width) * ctx->fov;
        float rayX = cos(rayAngle * M_PI / 180.0);
        float rayY = sin(rayAngle * M_PI / 180.0);

        float distance = 0.0;
        float stepSize = 0.05;
        float hitX = ctx->player->X, hitY = ctx->player->Y;
        bool hitWall = false;

        while (!hitWall && distance < 20.0) {
            hitX = ctx->player->X + rayX * distance;
            hitY = ctx->player->Y + rayY * distance;
            if (ctx->map[(int)hitY][(int)hitX] == 1) {
                hitWall = true;
            } else {
                distance += stepSize;
            }
        }

        int wallHeight = (int)(ctx->screen_height/ (distance + 0.1));
        SDL_SetRenderDrawColor(renderer, 255 / (distance * 0.1), 255 / (distance * 0.1), 255 / (distance * 0.1), 255);
        SDL_RenderDrawLine(renderer, x, (ctx->screen_height / 2) - wallHeight, x, (ctx->screen_height / 2) + wallHeight);
    }
}

