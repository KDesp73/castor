#include "raycaster.h"
#include "context.h"
#include "sprite.h"
#include "ui.h"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


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

void castor_CastWalls(SDL_Renderer *renderer, castor_Context* ctx)
{
    // Ensure the Z-buffer is allocated
    if (ctx->raycaster.z_buffer == NULL) {
        ctx->raycaster.z_buffer = (float*)malloc(ctx->sdl.screen_width * sizeof(float));
    }

    for (int x = 0; x < ctx->sdl.screen_width; x++) {
        float rayAngle = (ctx->level.player->angleX - ctx->settings.fov / 2.0) + ((float)x / ctx->sdl.screen_width) * ctx->settings.fov;
        float rayX = cos(rayAngle * M_PI / 180.0);
        float rayY = sin(rayAngle * M_PI / 180.0);

        float distance = 0.1;
        float stepSize = 0.01; // NOTE: IMPORTANT!
        float hitX = ctx->level.player->X, hitY = ctx->level.player->Y;
        bool hitWall = false;
        int tile = TILE_EMPTY;

        while (!hitWall && distance < ctx->settings.render_distance) {
            hitX = ctx->level.player->X + rayX * distance;
            hitY = ctx->level.player->Y + rayY * distance;

            int mapX = (int)floor(hitX);
            int mapY = (int)floor(hitY);

            if (mapX < 0 || mapX >= ctx->level.map_width || mapY < 0 || mapY >= ctx->level.map_height) {
                hitWall = true;
                break;
            }

            tile = ctx->level.map[mapY][mapX];
            if (tile > 0) {
                hitWall = true;
                break;
            }

            distance += stepSize;
        }

        // Precise fisheye correction
        float correctedDistance = distance * cos((x - ctx->sdl.screen_width / 2.0) * (ctx->settings.fov / ctx->sdl.screen_width) * M_PI / 180.0);

        // Populate the Z-buffer with the corrected distance
        ctx->raycaster.z_buffer[x] = correctedDistance;

        int wallHeight = (int)(ctx->sdl.screen_height / correctedDistance);
        int verticalOffset = (int)(ctx->level.player->angleY * 5.0);

        int wallTop = (ctx->sdl.screen_height / 2) - wallHeight - verticalOffset;
        int wallBottom = (ctx->sdl.screen_height / 2) + wallHeight - verticalOffset;
        if (hitWall && ctx->raycaster.textures[tile-1] != NULL) {
            // Precise texture coordinate calculation
            float texCoordinate = CalculateTextureCoordinate(hitX, hitY, rayX, rayY);

            // Ensure smooth texture mapping and flip the texture on x-axis
            int texX = (int)((1.0 - texCoordinate) * (ctx->raycaster.texture_width - 1));
            texX = fmax(0, fmin(texX, ctx->raycaster.texture_width - 1));

            SDL_Rect srcRect = { 
                texX,  // Precise X coordinate (flipped)
                0,     // Full texture height 
                1,     // Single pixel width
                ctx->raycaster.texture_height 
            };

            SDL_Rect dstRect = { 
                x, 
                wallTop, 
                1, 
                wallBottom - wallTop 
            };

            SDL_RenderCopy(renderer, ctx->raycaster.textures[tile-1], &srcRect, &dstRect);
        } else {
            switch (tile % 11) {
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
                default: SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(UI_COLOR_GREEN)); break;
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

void castor_CastFloorAndCeiling(SDL_Renderer *renderer, const castor_Context *ctx)
{
    if (
        !ctx->raycaster.textures[ctx->raycaster.ceiling_texture_index] ||
        !ctx->raycaster.textures[ctx->raycaster.floor_texture_index]
    ) return;



    const float playerHeight = 0.5f;  // Player's eye level
    const float textureScale = 1.0f;  // Texture density

    // Player direction vectors
    float angle       = ctx->level.player->angleX;
    float angleRad    = angle * M_PI / 180.0f;
    float dirX        = cosf(angleRad);  // Player direction X
    float dirY        = sinf(angleRad);  // Player direction Y
    float aspectRatio = (float) ctx->sdl.screen_width / ctx->sdl.screen_height;
    float fov_rad     = ctx->settings.fov * M_PI / 180.0f;
    float planeScale  = tanf(fov_rad / 2.0f) * aspectRatio;
    float planeX      = -dirY * planeScale;
    float planeY      = dirX * planeScale;

    printf("angle: %f\n", angle);
    printf("dirX: %f\n", dirX);
    printf("dirY: %f\n", dirY);

    int horizon = (ctx->sdl.screen_height / 2) - (int)(ctx->level.player->angleY * 5);

    // Create screen texture for pixel access
    SDL_Texture *screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_STREAMING, ctx->sdl.screen_width, ctx->sdl.screen_height);
    if (!screenTexture) return;

    Uint32 *pixels;
    int pitch;
    SDL_LockTexture(screenTexture, NULL, (void**)&pixels, &pitch);

    int floorW, floorH, ceilW, ceilH;
    Uint32* floorPixels = getPixels(renderer, ctx->raycaster.textures[ctx->raycaster.floor_texture_index], &floorW, &floorH);
    Uint32* ceilingPixels = getPixels(renderer, ctx->raycaster.textures[ctx->raycaster.ceiling_texture_index], &ceilW, &ceilH);

    for (int y = 0; y < ctx->sdl.screen_height; y++) {
        if (y == horizon) continue;

        bool isFloor = y > horizon;
        float vertPos = isFloor ? (y - horizon) : (horizon - y);
        float rowDistance = (playerHeight * ctx->sdl.screen_height) / (1e-5f + vertPos);

        // Static position calculation
        float floorStepX = rowDistance * (dirX - planeX);
        float floorStepY = rowDistance * (dirY - planeY);

        // Texture stepping (perspective correct)
        float texStepX = rowDistance * (2.0f * planeX) / ctx->sdl.screen_width;
        float texStepY = rowDistance * (2.0f * planeY) / ctx->sdl.screen_width;

        // Initial texture coordinates
        float floorX = ctx->level.player->X + floorStepX * 2.0f; // Correct
        float floorY = ctx->level.player->Y + floorStepY;

        // Texture selection
        Uint32 *texPixels = isFloor ? floorPixels : ceilingPixels;
        int texW = isFloor ? floorW : ceilW;
        int texH = isFloor ? floorH : ceilH;

        for (int x = 0; x < ctx->sdl.screen_width; x++) {
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

static bool renderSprite(castor_Context* ctx, castor_Sprite sprite, double dirX, double dirY, double planeX, double planeY) 
{
    SDL_Texture* spriteTexture = ctx->raycaster.sprite_textures[sprite.texture_id];
    if (!spriteTexture) return false;

    // Get texture dimensions and calculate aspect ratio
    int texW, texH;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &texW, &texH);
    float aspect_ratio = (float)texW / texH;  // Preserve aspect ratio

    // Translate sprite position
    double spriteX = sprite.x - ctx->level.player->X;
    double spriteY = sprite.y - ctx->level.player->Y;

    // Transform to camera space
    double invDet = 1.0 / (planeX * dirY - dirX * planeY);
    double transformX = invDet * (dirY * spriteX - dirX * spriteY);
    double transformY = invDet * (-planeY * spriteX + planeX * spriteY);
    if (transformY <= 0.01) return false;

    // Calculate sprite height based on distance and scale
    int spriteHeight = abs((int)(ctx->sdl.screen_height / transformY * sprite.scale));
    int spriteWidth = (int)(spriteHeight * aspect_ratio);  // Width derived from height and aspect ratio

    // Skip rendering if too small
    if (spriteHeight < 2 || spriteWidth < 2) return false;;

    // Calculate screen position
    int spriteScreenX = (int)(ctx->sdl.screen_width / 2 * (1 + transformX / transformY));

    // Vertical positioning
    double verticalOffset = sprite.z / transformY;
    int baseScreenY = (int)(ctx->sdl.screen_height / 2 - ctx->level.player->angleY * 5 - verticalOffset * 100);

    // Calculate drawing bounds
    int drawStartX = spriteScreenX - spriteWidth / 2;
    int drawEndX = spriteScreenX + spriteWidth / 2;
    int drawStartY = baseScreenY - spriteHeight / 2;
    int drawEndY = baseScreenY + spriteHeight / 2;

    // Render vertical stripes (only visible parts will be rendered)
    SDL_SetTextureBlendMode(spriteTexture, SDL_BLENDMODE_BLEND);
    for (int x = drawStartX; x < drawEndX; x++) {
        // Skip if the stripe is outside the screen or occluded by the z-buffer
        if (x < 0 || x >= ctx->sdl.screen_width || transformY >= ctx->raycaster.z_buffer[x]) continue;

        // Calculate texture X coordinate proportionally
        int texX = (int)((x - drawStartX) * (float)texW / (drawEndX - drawStartX));
        texX = texX < 0 ? 0 : (texX >= texW ? texW - 1 : texX);

        // Render the entire vertical stripe (no clamping for height)
        SDL_Rect srcRect = {texX, 0, 1, texH};
        SDL_Rect destRect = {x, drawStartY, 1, drawEndY - drawStartY};
        SDL_RenderCopy(ctx->sdl.renderer, spriteTexture, &srcRect, &destRect);
    }

    return true;
}

void castor_CastSprites(SDL_Renderer* renderer, castor_Context* ctx)
{
    castor_Player* player = ctx->level.player;

    // Precompute camera vectors
    double dirX = cos(player->angleX * M_PI / 180.0);
    double dirY = sin(player->angleX * M_PI / 180.0);
    double planeX = -dirY * tan(ctx->settings.fov * M_PI / 360.0);
    double planeY = dirX * tan(ctx->settings.fov * M_PI / 360.0);

    castor_Sprite* list[ctx->level.sprite_count];
    memcpy(list, ctx->level.sprites, ctx->level.sprite_count * sizeof(castor_Sprite*));

    for (int i = 0; i < ctx->level.sprite_count; i++) {
        castor_Sprite* sprite = list[i];
        if(!sprite) continue;
        double dx = sprite->x - player->X;
        double dy = sprite->y - player->Y;
        sprite->distance = dx * dx + dy * dy;
    }

    qsort(list, ctx->level.sprite_count, sizeof(castor_Sprite*), castor_SpriteCmp);

    for (int i = 0; i < ctx->level.sprite_count; i++) {
        castor_Sprite* sprite = list[i];
        if(!renderSprite(ctx, *sprite, dirX, dirY, planeX, planeY)) continue;
    }
}
