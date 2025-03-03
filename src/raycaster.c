#include "raycaster.h"
#include "context.h"
#include <SDL2/SDL_render.h>

#include <stdbool.h>

void CastRays(SDL_Renderer *renderer, const Context* ctx)
{
    for (int x = 0; x < ctx->screen_width; x++) {
        float rayAngle = (ctx->player->angle - ctx->fov / 2.0) + ((float)x / ctx->screen_width) * ctx->fov;
        float rayX = cos(rayAngle * M_PI / 180.0);
        float rayY = sin(rayAngle * M_PI / 180.0);

        float distance = 0.1;
        float stepSize = 0.05;
        float hitX = ctx->player->X, hitY = ctx->player->Y;
        bool hitWall = false;

        while (!hitWall && distance < 20.0) {
            hitX = ctx->player->X + rayX * distance;
            hitY = ctx->player->Y + rayY * distance;

            int mapX = (int)hitX;
            int mapY = (int)hitY;

            if (mapX < 0 || mapX >= ctx->map_width || mapY < 0 || mapY >= ctx->map_height) {
                hitWall = true;
                break;
            }

            if (ctx->map[mapY][mapX] == 1) {
                hitWall = true;
                break;
            }

            distance += stepSize;
        }

        float correctedDistance = distance * cos((rayAngle - ctx->player->angle) * M_PI / 180.0);
        if (correctedDistance < 0.1) correctedDistance = 0.1;

        int color = 255 - (correctedDistance * 10);
        if (color < 0) color = 0;

        int wallHeight = (int)(ctx->screen_height / correctedDistance);

        SDL_SetRenderDrawColor(renderer, color, color, color, 255);
        SDL_RenderDrawLine(renderer, x, (ctx->screen_height / 2) - wallHeight, x, (ctx->screen_height / 2) + wallHeight);
    }
}

