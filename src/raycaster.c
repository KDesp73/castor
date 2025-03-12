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

void CastSprites(SDL_Renderer* renderer, const Context* ctx) {
    Player* player = ctx->player;
    int screen_width = ctx->screen_width;
    int screen_height = ctx->screen_height;
    float* z_buffer = ctx->z_buffer;

    double dirX = cos(player->angleX * M_PI / 180.0);
    double dirY = sin(player->angleX * M_PI / 180.0);
    double planeX = -dirY * tan(ctx->fov * M_PI / 360.0);
    double planeY = dirX * tan(ctx->fov * M_PI / 360.0);

    // Sort sprites by distance (farthest to closest)
    qsort((Sprite*)ctx->sprites, ctx->sprite_count, sizeof(Sprite), SpriteCmp);

    // Pre-cache texture data
    SDL_Texture* spriteTextures[ctx->sprite_count];
    int textureWidths[ctx->sprite_count];
    int textureHeights[ctx->sprite_count];

    for (int i = 0; i < ctx->sprite_count; i++) {
        Sprite sprite = ctx->sprites[i];
        spriteTextures[i] = ctx->sprite_textures[sprite.texture_id];
        if (spriteTextures[i]) {
            SDL_QueryTexture(spriteTextures[i], NULL, NULL, &textureWidths[i], &textureHeights[i]);
        }
    }

    // Precalculate camera inverse determinant
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    int halfWidth = screen_width / 2;
    int halfHeight = screen_height / 2;

    for (int i = 0; i < ctx->sprite_count; i++) {
        Sprite sprite = ctx->sprites[i];
        SDL_Texture* spriteTexture = spriteTextures[i];

        if (!spriteTexture) continue;

        // Translate sprite position relative to the player
        double spriteX = sprite.x - player->X;
        double spriteY = sprite.y - player->Y;

        // Transform sprite with the inverse camera matrix
        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY);

        // Skip sprites behind the camera or too far away
        if (transformY <= 0.01) continue;

        // Calculate sprite dimensions and position on screen
        int spriteScreenX = (int)(halfWidth * (1 + transformX / transformY));

        // Apply sprite scale
        int spriteHeight = abs((int)(screen_height / transformY * sprite.scale));
        int spriteWidth = abs((int)(screen_height / transformY * sprite.scale));

        // Early out for very small sprites
        if (spriteHeight < 2 || spriteWidth < 2) continue;

        // Calculate vertical position with z-offset
        double verticalOffset = sprite.z / transformY;
        int baseScreenY = (int)(halfHeight - player->angleY * 5 - verticalOffset * 100);

        // Calculate screen boundaries
        int drawStartY = -spriteHeight / 2 + baseScreenY;
        int drawEndY = spriteHeight / 2 + baseScreenY;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        int drawEndX = spriteWidth / 2 + spriteScreenX;

        // Clamp to screen bounds
        if (drawStartY < 0) drawStartY = 0;
        if (drawEndY >= screen_height) drawEndY = screen_height - 1;
        if (drawStartX < 0) drawStartX = 0;
        if (drawEndX >= screen_width) drawEndX = screen_width - 1;

        if (drawStartX >= drawEndX || drawStartY >= drawEndY) continue;

        int texture_width = textureWidths[i];
        int texture_height = textureHeights[i];

        // Loop through every vertical stripe of the sprite on the screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            // Calculate the texture X coordinate
            int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texture_width / spriteWidth / 256);

            // Clamp texX to valid bounds
            if (texX < 0) texX = 0;
            if (texX >= texture_width) texX = texture_width - 1;

            // Check if this stripe is visible in the z-buffer
            if (transformY > 0 && stripe >= 0 && stripe < screen_width && transformY < z_buffer[stripe]) {
                // For each vertical pixel in the stripe
                for (int y = drawStartY; y < drawEndY; y++) {
                    // Use original precise texture coordinate calculation
                    int d = (y - baseScreenY) * 256 + spriteHeight * 128;
                    int texY = ((d * texture_height) / spriteHeight) / 256;

                    // Clamp texY to valid bounds
                    if (texY < 0) texY = 0;
                    if (texY >= texture_height) texY = texture_height - 1;

                    // Render the pixel
                    SDL_Rect srcRect = { texX, texY, 1, 1 };
                    SDL_Rect destRect = { stripe, y, 1, 1 };
                    SDL_RenderCopy(renderer, spriteTexture, &srcRect, &destRect);
                }
            }
        }
    }
}
