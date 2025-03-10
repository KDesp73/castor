#include "raycaster.h"
#include "context.h"
#include "ui.h"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>

#include <stdbool.h>
#include <stdio.h>

static float CalculateTextureCoordinate(float hitX, float hitY, float rayX, float rayY)
{
    // Detect exactly which wall was hit by checking grid boundaries
    float fracX = hitX - floor(hitX);
    float fracY = hitY - floor(hitY);
    float texCoord;

    bool hitVerticalWall = false;

    if (fracX < 0.01f || fracX > 0.99f) {
        hitVerticalWall = true;
    }
    else if (fracY < 0.01f || fracY > 0.99f) {
        hitVerticalWall = false;
    }
    else {
        hitVerticalWall = (fabs(rayX) >= fabs(rayY));
    }

    if (hitVerticalWall) {
        texCoord = fracY;

        if (rayX > 0) {
            texCoord = 1.0f - texCoord;
        }
    } else {
        texCoord = fracX;

        if (rayY < 0) {
            texCoord = 1.0f - texCoord;
        }
    }

    return fmaxf(0.0f, fminf(0.999f, texCoord));
}

void CastRays(SDL_Renderer *renderer, const Context* ctx)
{
    for (int x = 0; x < ctx->screen_width; x++) {
        float rayAngle = (ctx->player->angleX - ctx->fov / 2.0) + ((float)x / ctx->screen_width) * ctx->fov;
        float rayX = cos(rayAngle * M_PI / 180.0);
        float rayY = sin(rayAngle * M_PI / 180.0);

        float distance = 0.1;
        float stepSize = 0.01; // NOTE: IMPORTANT!
        float hitX = ctx->player->X, hitY = ctx->player->Y;
        bool hitWall = false;
        Tile tile = TILE_EMPTY;

        while (!hitWall && distance < 20.0) {
            hitX = ctx->player->X + rayX * distance;
            hitY = ctx->player->Y + rayY * distance;

            int mapX = (int)floor(hitX);
            int mapY = (int)floor(hitY);

            if (mapX < 0 || mapX >= ctx->map_width || mapY < 0 || mapY >= ctx->map_height) {
                hitWall = true;
                break;
            }

            tile = (Tile)ctx->map[mapY][mapX];
            if (tile > 0) {
                hitWall = true;
                break;
            }

            distance += stepSize;
        }

        // Precise fisheye correction
        float correctedDistance = distance * cos((x - ctx->screen_width / 2.0) * (ctx->fov / ctx->screen_width) * M_PI / 180.0);

        int wallHeight = (int)(ctx->screen_height / correctedDistance);
        int verticalOffset = (int)(ctx->player->angleY * 5.0);

        int wallTop = (ctx->screen_height / 2) - wallHeight - verticalOffset;
        int wallBottom = (ctx->screen_height / 2) + wallHeight - verticalOffset;
        if (hitWall && ctx->textures[tile] != NULL) {
            // Precise texture coordinate calculation
            float texCoordinate = CalculateTextureCoordinate(hitX, hitY, rayX, rayY);

            // Ensure smooth texture mapping
            int texX = (int)(texCoordinate * (ctx->texture_width - 1));
            texX = fmax(0, fmin(texX, ctx->texture_width - 1));

            SDL_Rect srcRect = { 
                texX,  // Precise X coordinate 
                0,     // Full texture height 
                1,     // Single pixel width
                ctx->texture_height 
            };

            SDL_Rect dstRect = { 
                x, 
                wallTop, 
                1, 
                wallBottom - wallTop 
            };

            SDL_RenderCopy(renderer, ctx->textures[tile], &srcRect, &dstRect);
        } else {
            switch (tile % 9) {
                case 1: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_RED)); break;
                case 2: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_GREEN)); break;
                case 3: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_BLUE)); break;
                case 4: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_YELLOW)); break;
                case 5: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_MAGENTA)); break;
                case 6: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_ORANGE)); break;
                case 7: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_PINK)); break;
                case 8: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_TEAL)); break;
                default: SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); break;
            }
            SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom); 
        }
    }
}

void DrawFloorAndCeiling(SDL_Renderer *renderer, const Context *ctx)
{
    const float playerHeight = 0.5f;
    const float baseTextureScale = 1.0f;  // Base scale for 1:1 texture/world ratio
    float textureScale = 1.0f;  // Adjust this value to control texture size

    // Player direction vectors
    float angleRad = ctx->player->angleX * M_PI / 180.0f;
    float dirX = cosf(angleRad);
    float dirY = sinf(angleRad);

    // Camera plane with aspect ratio correction
    float aspectRatio = (float)ctx->screen_width / ctx->screen_height;
    float fov_rad = ctx->fov * M_PI / 180.0f;
    float planeScale = tanf(fov_rad / 2.0f) * aspectRatio;
    float planeX = -dirY * planeScale;
    float planeY = dirX * planeScale;

    // Horizon calculation (original working version)
    int horizon = (ctx->screen_height / 2) - (int)(ctx->player->angleY * 5);

    // Create screen texture for pixel access
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STREAMING, ctx->screen_width, ctx->screen_height);
    if (!screenTexture) return;

    Uint32 *pixels;
    int pitch;
    SDL_LockTexture(screenTexture, NULL, (void**)&pixels, &pitch);

    // PROPER TEXTURE PIXEL EXTRACTION
    SDL_Texture *floorTexture = ctx->textures[ctx->floor_texture_index];
    SDL_Texture *ceilingTexture = ctx->textures[ctx->ceiling_texture_index];

    // Get floor texture pixels
    int floorW, floorH;
    SDL_QueryTexture(floorTexture, NULL, NULL, &floorW, &floorH);
    SDL_Texture *floorTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, floorW, floorH);
    SDL_SetRenderTarget(renderer, floorTarget);
    SDL_RenderCopy(renderer, floorTexture, NULL, NULL);
    Uint32 *floorPixels = malloc(floorW * floorH * sizeof(Uint32));
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, floorPixels, floorW * sizeof(Uint32));
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(floorTarget);

    // Get ceiling texture pixels (same process)
    int ceilW, ceilH;
    SDL_QueryTexture(ceilingTexture, NULL, NULL, &ceilW, &ceilH);
    SDL_Texture *ceilTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, ceilW, ceilH);
    SDL_SetRenderTarget(renderer, ceilTarget);
    SDL_RenderCopy(renderer, ceilingTexture, NULL, NULL);
    Uint32 *ceilPixels = malloc(ceilW * ceilH * sizeof(Uint32));
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, ceilPixels, ceilW * sizeof(Uint32));
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(ceilTarget);

    // Floor/ceiling projection loop
    for (int y = 0; y < ctx->screen_height; y++) {
        if (y == horizon) continue;

        bool isFloor = y > horizon;
        float vertPos = isFloor ? (y - horizon) : (horizon - y);
        float rowDistance = (playerHeight * ctx->screen_height) / (1e-5f + vertPos);

        // Calculate real world coordinates with dual scaling
        float floorStepX = rowDistance * (dirX - planeX) * baseTextureScale;
        float floorStepY = rowDistance * (dirY - planeY) * baseTextureScale;
        float floorX = ctx->player->X * textureScale + floorStepX;
        float floorY = ctx->player->Y * textureScale + floorStepY;

        // Scaled stepping increments
        float texStepX = rowDistance * (2.0f * planeX) / ctx->screen_width * textureScale;
        float texStepY = rowDistance * (2.0f * planeY) / ctx->screen_width * textureScale;

        // Texture selection
        Uint32 *texPixels = isFloor ? floorPixels : ceilPixels;
        int texW = isFloor ? floorW : ceilW;
        int texH = isFloor ? floorH : ceilH;

        for (int x = 0; x < ctx->screen_width; x++) {
            // Calculate texture coordinates with proper scaling
            int texX = (int)(floorX * texW / textureScale) % texW;
            int texY = (int)(floorY * texH / textureScale) % texH;
            texX = texX < 0 ? texX + texW : texX;
            texY = texY < 0 ? texY + texH : texY;

            pixels[y * (pitch/sizeof(Uint32)) + x] = texPixels[texY * texW + texX];

            floorX += texStepX;
            floorY += texStepY;
        }
    }

    // Cleanup
    SDL_UnlockTexture(screenTexture);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_DestroyTexture(screenTexture);
    free(floorPixels);
    free(ceilPixels);
}
