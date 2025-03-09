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
    // Precompute player direction and camera plane
    float dirX = cos(ctx->player->angleX * M_PI / 180.0);
    float dirY = sin(ctx->player->angleX * M_PI / 180.0);
    float planeX = -dirY * ctx->fov;
    float planeY = dirX * ctx->fov;
    
    // Adjust horizon based on vertical angle
    int horizon = (ctx->screen_height / 2) - (int)(ctx->player->angleY * 5);
    
    // Get textures
    SDL_Texture *floorTexture = ctx->textures[ctx->floor_texture_index];
    SDL_Texture *ceilingTexture = ctx->textures[ctx->ceiling_texture_index];
    
    // Create target texture for the entire screen
    SDL_Texture *screenTexture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        ctx->screen_width, 
        ctx->screen_height
    );
    
    if (!screenTexture) {
        return; // Failed to create texture
    }
    
    // Lock the texture to get direct pixel access
    Uint32 *pixels = NULL;
    int pitch = 0;
    if (SDL_LockTexture(screenTexture, NULL, (void**)&pixels, &pitch) != 0) {
        SDL_DestroyTexture(screenTexture);
        return; // Failed to lock texture
    }
    
    // Get texture dimensions
    int floorTexWidth, floorTexHeight;
    int ceilingTexWidth, ceilingTexHeight;
    SDL_QueryTexture(floorTexture, NULL, NULL, &floorTexWidth, &floorTexHeight);
    SDL_QueryTexture(ceilingTexture, NULL, NULL, &ceilingTexWidth, &ceilingTexHeight);
    
    // For floor texture
    SDL_Surface *floorSurface = SDL_CreateRGBSurfaceWithFormat(0, floorTexWidth, floorTexHeight, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!floorSurface) {
        SDL_UnlockTexture(screenTexture);
        SDL_DestroyTexture(screenTexture);
        return;
    }
    
    // Save current render target
    SDL_Texture *oldTarget = SDL_GetRenderTarget(renderer);
    
    // Render the floor texture to a temporary target
    SDL_Texture *tempTarget = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        floorTexWidth, 
        floorTexHeight
    );
    
    if (!tempTarget) {
        SDL_FreeSurface(floorSurface);
        SDL_UnlockTexture(screenTexture);
        SDL_DestroyTexture(screenTexture);
        return;
    }
    
    // Set the temporary texture as render target and copy floor texture to it
    SDL_SetRenderTarget(renderer, tempTarget);
    SDL_RenderCopy(renderer, floorTexture, NULL, NULL);
    
    // Read the pixels from the render target
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, floorSurface->pixels, floorSurface->pitch);
    
    // For ceiling texture
    SDL_Surface *ceilingSurface = SDL_CreateRGBSurfaceWithFormat(0, ceilingTexWidth, ceilingTexHeight, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!ceilingSurface) {
        SDL_FreeSurface(floorSurface);
        SDL_DestroyTexture(tempTarget);
        SDL_SetRenderTarget(renderer, oldTarget);
        SDL_UnlockTexture(screenTexture);
        SDL_DestroyTexture(screenTexture);
        return;
    }
    
    // Clear the render target and copy ceiling texture to it
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, ceilingTexture, NULL, NULL);
    
    // Read the pixels from the render target
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, ceilingSurface->pixels, ceilingSurface->pitch);
    
    // Restore the original render target
    SDL_SetRenderTarget(renderer, oldTarget);
    SDL_DestroyTexture(tempTarget);
    
    // Cast rays for each vertical scanline
    float posZ = 0.5 * ctx->screen_height; // Camera height
    
    for (int y = 0; y < ctx->screen_height; y++) {
        // Skip the horizon line to avoid division by zero
        if (y == horizon) continue;
        
        // Determine if we're drawing floor or ceiling
        bool isFloor = y > horizon;
        
        // Calculate distance from horizon
        int p = isFloor ? (y - horizon) : (horizon - y);
        if (p <= 0) continue;
        
        // Calculate row distance
        float rowDistance = posZ / p;
        
        // Calculate ray directions
        float rayDirX0 = dirX - planeX;
        float rayDirY0 = dirY - planeY;
        float rayDirX1 = dirX + planeX;
        float rayDirY1 = dirY + planeY;
        
        // Step sizes
        float stepX = rowDistance * (rayDirX1 - rayDirX0) / ctx->screen_width;
        float stepY = rowDistance * (rayDirY1 - rayDirY0) / ctx->screen_width;
        
        // Starting position
        float worldX = ctx->player->X + rowDistance * rayDirX0;
        float worldY = ctx->player->Y + rowDistance * rayDirY0;
        
        // Select the appropriate texture and dimensions
        SDL_Surface *textureSurface = isFloor ? floorSurface : ceilingSurface;
        int texWidth = isFloor ? floorTexWidth : ceilingTexWidth;
        int texHeight = isFloor ? floorTexHeight : ceilingTexHeight;
        
        // Ensure the dimensions are power of two for proper wrapping
        bool isPowerOfTwo = (texWidth & (texWidth - 1)) == 0 && (texHeight & (texHeight - 1)) == 0;
        
        for (int x = 0; x < ctx->screen_width; x++) {
            // Calculate texture coordinates
            int cellX = (int)worldX;
            int cellY = (int)worldY;
            
            float texX_f = (worldX - cellX) * texWidth;
            float texY_f = (worldY - cellY) * texHeight;
            
            // Apply proper wrapping based on texture dimensions
            int texX, texY;
            if (isPowerOfTwo) {
                texX = ((int)texX_f) & (texWidth - 1);
                texY = ((int)texY_f) & (texHeight - 1);
            } else {
                texX = ((int)texX_f) % texWidth;
                texY = ((int)texY_f) % texHeight;
                
                // Ensure positive values for modulo
                if (texX < 0) texX += texWidth;
                if (texY < 0) texY += texHeight;
            }
            
            // Get pixel from texture surface
            Uint32 *surfacePixels = (Uint32*)textureSurface->pixels;
            Uint32 color = surfacePixels[texY * texWidth + texX];
            
            // Set pixel in the target texture
            pixels[y * (pitch / sizeof(Uint32)) + x] = color;
            
            // Advance to next world position
            worldX += stepX;
            worldY += stepY;
        }
    }
    
    // Unlock texture
    SDL_UnlockTexture(screenTexture);
    
    // Render the texture to the screen
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    
    // Clean up
    SDL_DestroyTexture(screenTexture);
    SDL_FreeSurface(floorSurface);
    SDL_FreeSurface(ceilingSurface);
}
