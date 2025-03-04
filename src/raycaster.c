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
        Tile tile = TILE_EMPTY;
        float textureOffset = 0.0f;

        while (!hitWall && distance < 20.0) {
            hitX = ctx->player->X + rayX * distance;
            hitY = ctx->player->Y + rayY * distance;

            int mapX = (int)hitX;
            int mapY = (int)hitY;

            if (mapX < 0 || mapX >= ctx->map_width || mapY < 0 || mapY >= ctx->map_height) {
                hitWall = true;
                break;
            }

            tile = (Tile) ctx->map[mapY][mapX];
            if (tile > 0) {
                hitWall = true;

                // Calculate texture coordinate (0 to 1)
                if (fmod(hitX, 1.0) < fmod(hitY, 1.0)) {
                    textureOffset = fmod(hitX, 1.0);
                } else {
                    textureOffset = fmod(hitY, 1.0);
                }
            }

            distance += stepSize;
        }

        float correctedDistance = distance * cos((rayAngle - ctx->player->angleX) * M_PI / 180.0);
        if (correctedDistance < 0.1) correctedDistance = 0.1;

        int wallHeight = (int)(ctx->screen_height / correctedDistance);
        int verticalOffset = (int)(ctx->player->angleY * 5.0); // Adjust based on angleY
        int wallTop = (ctx->screen_height / 2) - wallHeight - verticalOffset;
        int wallBottom = (ctx->screen_height / 2) + wallHeight - verticalOffset;

        if (ctx->textures[tile] != NULL) {
            // Render texture slice
            SDL_Rect srcRect = { (int)(textureOffset * ctx->texture_width), 0, 1, ctx->texture_height };
            SDL_Rect dstRect = { x, wallTop, 1, wallBottom - wallTop };

            SDL_RenderCopy(renderer, ctx->textures[tile], &srcRect, &dstRect);
        } else {
            // Fallback to solid colors if texture is missing
            switch (tile) {
                case TILE_WHITE: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
                case TILE_BLACK: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
                case TILE_RED: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                case TILE_BLUE: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
                case TILE_GREEN: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
                default: SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); break;
            }
            SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom);
        }
    }
}
