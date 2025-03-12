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

void CastWalls(SDL_Renderer *renderer, Context* ctx)
{
    // Ensure the Z-buffer is allocated
    if (ctx->z_buffer == NULL) {
        ctx->z_buffer = (float*)malloc(ctx->screen_width * sizeof(float));
    }

    for (int x = 0; x < ctx->screen_width; x++) {
        float rayAngle = (ctx->player->angleX - ctx->fov / 2.0) + ((float)x / ctx->screen_width) * ctx->fov;
        float rayX = cos(rayAngle * M_PI / 180.0);
        float rayY = sin(rayAngle * M_PI / 180.0);

        float distance = 0.1;
        float stepSize = 0.01; // NOTE: IMPORTANT!
        float hitX = ctx->player->X, hitY = ctx->player->Y;
        bool hitWall = false;
        int tile = TILE_EMPTY;

        while (!hitWall && distance < ctx->render_distance) {
            hitX = ctx->player->X + rayX * distance;
            hitY = ctx->player->Y + rayY * distance;

            int mapX = (int)floor(hitX);
            int mapY = (int)floor(hitY);

            if (mapX < 0 || mapX >= ctx->map_width || mapY < 0 || mapY >= ctx->map_height) {
                hitWall = true;
                break;
            }

            tile = ctx->map[mapY][mapX];
            if (tile > 0) {
                hitWall = true;
                break;
            }

            distance += stepSize;
        }

        // Precise fisheye correction
        float correctedDistance = distance * cos((x - ctx->screen_width / 2.0) * (ctx->fov / ctx->screen_width) * M_PI / 180.0);

        // Populate the Z-buffer with the corrected distance
        ctx->z_buffer[x] = correctedDistance;

        int wallHeight = (int)(ctx->screen_height / correctedDistance);
        int verticalOffset = (int)(ctx->player->angleY * 5.0);

        int wallTop = (ctx->screen_height / 2) - wallHeight - verticalOffset;
        int wallBottom = (ctx->screen_height / 2) + wallHeight - verticalOffset;
        if (hitWall && ctx->textures[tile-1] != NULL) {
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

            SDL_RenderCopy(renderer, ctx->textures[tile-1], &srcRect, &dstRect);
        } else {
            switch (tile) {
                case 1: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_RED)); break;
                case 2: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_GREEN)); break;
                case 3: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_BLUE)); break;
                case 4: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_YELLOW)); break;
                case 5: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_MAGENTA)); break;
                case 6: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_ORANGE)); break;
                case 7: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_PINK)); break;
                case 8: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_TEAL)); break;
                case 9: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_OLIVE)); break;
                case 10: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_BRONZE)); break;
                // default: SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); break;
            }
            SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom); 
        }
    }
}

static Uint32* getPixels(SDL_Renderer* renderer, SDL_Texture* texture, int* W, int* H)
{
    SDL_QueryTexture(texture, NULL, NULL, W, H);
    SDL_Texture *target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, *W, *H);
    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    Uint32 *pixels = malloc(*W * *H * sizeof(Uint32));
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, *W * sizeof(Uint32));
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(target);
    
    return pixels;
}

void CastFloorAndCeiling(SDL_Renderer *renderer, const Context *ctx)
{
    if (!ctx->textures[ctx->ceiling_texture_index] || !ctx->textures[ctx->floor_texture_index]) return;

    const float playerHeight = 0.5f;  // Player's eye level
    const float textureScale = 1.0f;  // Texture density

    // Player direction vectors
    float angleRad    = ctx->player->angleX * M_PI / 180.0f;
    float dirX        = cosf(angleRad);  // Player direction X
    float dirY        = sinf(angleRad);  // Player direction Y
    float aspectRatio = (float) ctx->screen_width / ctx->screen_height;
    float fov_rad     = ctx->fov * M_PI / 180.0f;
    float planeScale  = tanf(fov_rad / 2.0f) * aspectRatio;
    float planeX      = -dirY * planeScale;  // Right vector X (flipped sign)
    float planeY      = dirX * planeScale;   // Right vector Y (flipped sign)

    int horizon = (ctx->screen_height / 2) - (int)(ctx->player->angleY * 5);

    // Create screen texture for pixel access
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STREAMING, ctx->screen_width, ctx->screen_height);
    if (!screenTexture) return;

    Uint32 *pixels;
    int pitch;
    SDL_LockTexture(screenTexture, NULL, (void**)&pixels, &pitch);

    int floorW, floorH, ceilW, ceilH;
    Uint32* floorPixels = getPixels(renderer, ctx->textures[ctx->floor_texture_index], &floorW, &floorH);
    Uint32* ceilingPixels = getPixels(renderer, ctx->textures[ctx->ceiling_texture_index], &ceilW, &ceilH);

    for (int y = 0; y < ctx->screen_height; y++) {
        if (y == horizon) continue;

        bool isFloor = y > horizon;
        float vertPos = isFloor ? (y - horizon) : (horizon - y);
        float rowDistance = (playerHeight * ctx->screen_height) / (1e-5f + vertPos);

        // Static position calculation
        float floorStepX = rowDistance * (dirX - planeX);
        float floorStepY = rowDistance * (dirY - planeY);

        // Texture stepping (perspective correct)
        float texStepX = rowDistance * (2.0f * planeX) / ctx->screen_width;
        float texStepY = rowDistance * (2.0f * planeY) / ctx->screen_width;

        // Initial texture coordinates
        float floorX = ctx->player->X + floorStepX * 2.0f; // Correct
        float floorY = ctx->player->Y + floorStepY;

        // Texture selection
        Uint32 *texPixels = isFloor ? floorPixels : ceilingPixels;
        int texW = isFloor ? floorW : ceilW;
        int texH = isFloor ? floorH : ceilH;

        for (int x = 0; x < ctx->screen_width; x++) {
            // Calculate texture coordinates
            int texX = (int)(floorX * texW * textureScale) % texW;
            int texY = (int)(floorY * texH * textureScale) % texH;

            texX = texX < 0 ? texX + texW : texX;
            texY = texY < 0 ? texY + texH : texY;

            pixels[y * (pitch/sizeof(Uint32)) + x] = texPixels[texY * texW + texX];

            // Update coordinates using perspective-correct steps
            floorX += texStepX;
            floorY += texStepY;
        }
    }

    // Cleanup code
    SDL_UnlockTexture(screenTexture);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_DestroyTexture(screenTexture);
    free(ceilingPixels);
    free(floorPixels);
}

// static int Clamp(int value, int min, int max)
// {
//     if (value < min) {
//         return min;
//     } else if (value > max) {
//         return max;
//     }
//     return value;
// }

// static bool isSpriteVisible(int drawStartX, int drawEndX, double transformY, int screen_width, float* z_buffer)
// {
//     int center = (drawStartX + drawEndX) / 2;
//     int samples[3] = { drawStartX, center, drawEndX - 1 };
//     for (int j = 0; j < 3; j++) {
//         int stripe = samples[j];
//         if (stripe >= 0 && stripe < screen_width && transformY < z_buffer[stripe])
//             return true;
//     }
//     return false;
// }

void CastSprites(SDL_Renderer* renderer, Context* ctx)
{
    Player* player = ctx->player;
    int screen_width = ctx->screen_width;
    int screen_height = ctx->screen_height;
    float* z_buffer = ctx->z_buffer;

    // Precompute camera vectors
    double dirX = cos(player->angleX * M_PI / 180.0);
    double dirY = sin(player->angleX * M_PI / 180.0);
    double planeX = -dirY * tan(ctx->fov * M_PI / 360.0);
    double planeY = dirX * tan(ctx->fov * M_PI / 360.0);

    for (int i = 0; i < ctx->sprite_count; i++) {
        Sprite* sprite = &ctx->sprites[i];
        double dx = sprite->x - player->X;
        double dy = sprite->y - player->Y;
        sprite->distance = dx * dx + dy * dy;
    }

    // Sort sprites by distance (farthest to closest)
    qsort((Sprite*)ctx->sprites, ctx->sprite_count, sizeof(Sprite), SpriteCmp);

    for (int i = 0; i < ctx->sprite_count; i++) {
        Sprite sprite = ctx->sprites[i];
        SDL_Texture* spriteTexture = ctx->sprite_textures[sprite.texture_id];
        if (!spriteTexture) continue;

        // Get texture dimensions and calculate aspect ratio
        int texW, texH;
        SDL_QueryTexture(spriteTexture, NULL, NULL, &texW, &texH);
        float aspect_ratio = (float)texW / texH;  // Preserve aspect ratio

        // Translate sprite position
        double spriteX = sprite.x - player->X;
        double spriteY = sprite.y - player->Y;

        // Transform to camera space
        double invDet = 1.0 / (planeX * dirY - dirX * planeY);
        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
        if (transformY <= 0.01) continue;  // Skip sprites behind the camera

        // Calculate sprite height based on distance and scale
        int spriteHeight = abs((int)(screen_height / transformY * sprite.scale));
        int spriteWidth = (int)(spriteHeight * aspect_ratio);  // Width derived from height and aspect ratio

        // Skip rendering if too small
        if (spriteHeight < 2 || spriteWidth < 2) continue;

        // Calculate screen position
        int spriteScreenX = (int)(screen_width / 2 * (1 + transformX / transformY));

        // Vertical positioning
        double verticalOffset = sprite.z / transformY;
        int baseScreenY = (int)(screen_height / 2 - player->angleY * 5 - verticalOffset * 100);

        // Calculate drawing bounds (DO NOT CLAMP TO SCREEN BOUNDARIES)
        int drawStartX = spriteScreenX - spriteWidth / 2;
        int drawEndX = spriteScreenX + spriteWidth / 2;
        int drawStartY = baseScreenY - spriteHeight / 2;
        int drawEndY = baseScreenY + spriteHeight / 2;

        // Render vertical stripes (only visible parts will be rendered)
        SDL_SetTextureBlendMode(spriteTexture, SDL_BLENDMODE_BLEND);
        for (int x = drawStartX; x < drawEndX; x++) {
            // Skip if the stripe is outside the screen or occluded by the z-buffer
            if (x < 0 || x >= screen_width || transformY >= z_buffer[x]) continue;

            // Calculate texture X coordinate proportionally
            int texX = (int)((x - drawStartX) * (float)texW / (drawEndX - drawStartX));
            texX = texX < 0 ? 0 : (texX >= texW ? texW - 1 : texX);

            // Render the entire vertical stripe (no clamping for height)
            SDL_Rect srcRect = {texX, 0, 1, texH};
            SDL_Rect destRect = {x, drawStartY, 1, drawEndY - drawStartY};
            SDL_RenderCopy(renderer, spriteTexture, &srcRect, &destRect);
        }
    }
}
