#include "raycaster.h"
#include "context.h"
#include <SDL2/SDL_render.h>

#include <stdbool.h>

void CastRays(SDL_Renderer *renderer, const Context* ctx)
{
    for (int x = 0; x < ctx->screen_width; x++) {
        float rayAngle = (ctx->player->angleX - ctx->fov / 2.0) + ((float)x / ctx->screen_width) * ctx->fov;
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

            Tile tile = (Tile) ctx->map[mapY][mapX];
            if (tile > 0) {
                hitWall = true;

                switch (tile) {
                case TILE_WHITE:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
                case TILE_BLACK:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    break;
                case TILE_RED:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    break;
                case TILE_BLUE:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                case TILE_GREEN:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case TILE_EMPTY:
                default:
                    break;
                }
            }

            distance += stepSize;
        }

        float correctedDistance = distance * cos((rayAngle - ctx->player->angleX) * M_PI / 180.0);
        if (correctedDistance < 0.1) correctedDistance = 0.1;

        int color = 255 - (correctedDistance * 10);
        if (color < 0) color = 0;

        int wallHeight = (int)(ctx->screen_height / correctedDistance);

        // Apply vertical look adjustment using angleY
        int verticalOffset = (int)(ctx->player->angleY * 5.0); // Adjust scaling factor if needed
        int wallTop = (ctx->screen_height / 2) - wallHeight - verticalOffset;
        int wallBottom = (ctx->screen_height / 2) + wallHeight - verticalOffset;

        // Draw the line with the chosen color
        SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom);
    }
}
