#include "raycaster.h"
#include "context.h"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>

#include <stdbool.h>

static float CalculateTextureCoordinate(float hitX, float hitY, float rayX, float rayY)
{
    // Detect exactly which wall was hit by checking grid boundaries
    float fracX = hitX - floor(hitX);
    float fracY = hitY - floor(hitY);
    float texCoord;
    
    // Determine if we hit a vertical or horizontal wall
    bool hitVerticalWall = false;
    
    // Check if very close to a vertical grid line
    if (fracX < 0.01f || fracX > 0.99f) {
        hitVerticalWall = true;
    }
    // Check if very close to a horizontal grid line
    else if (fracY < 0.01f || fracY > 0.99f) {
        hitVerticalWall = false;
    }
    // If neither is definitive, use ray direction to determine
    else {
        // Determine based on which direction the ray travels more
        hitVerticalWall = (fabs(rayX) >= fabs(rayY));
    }
    
    // Calculate texture coordinate based on wall type
    if (hitVerticalWall) {
        // For vertical walls (running north-south)
        texCoord = fracY;
        
        // Flip texture based on which side of wall we hit
        if (rayX > 0) {
            texCoord = 1.0f - texCoord;
        }
    } else {
        // For horizontal walls (running east-west)
        texCoord = fracX;
        
        // Flip texture based on which side of wall we hit
        if (rayY < 0) {
            texCoord = 1.0f - texCoord;
        }
    }
    
    // Ensure texture coordinate is within valid range
    return fmaxf(0.0f, fminf(0.999f, texCoord));
}

// In your main rendering function
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
           switch (tile % 6) {
                case 1: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
                case 2: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
                case 3: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
                case 4: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
                case 5: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
                default: SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); break;
            }
            SDL_RenderDrawLine(renderer, x, wallTop, x, wallBottom); 
        }
    }
}

void DrawFloorAndCeiling(SDL_Renderer *renderer, const Context *ctx) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    // Camera direction and plane (for perspective-correct mapping)
    float dirX = cos(ctx->player->angleX * M_PI / 180.0);
    float dirY = sin(ctx->player->angleX * M_PI / 180.0);
    float planeX = -dirY * ctx->fov;
    float planeY = dirX * ctx->fov;

    for (int y = ctx->screen_height / 2; y < ctx->screen_height; y++) {
        float p = y - (ctx->screen_height / 2);
        float posZ = 0.5 * ctx->screen_height;
        float rowDistance = posZ / p;

        // Step increments for each x
        float stepX = rowDistance * (planeX * 2) / ctx->screen_width;
        float stepY = rowDistance * (planeY * 2) / ctx->screen_width;

        float floorX = ctx->player->X + rowDistance * (dirX - planeX);
        float floorY = ctx->player->Y + rowDistance * (dirY - planeY);

        for (int x = 0; x < ctx->screen_width; x++) {
            int mapX = (int)floorX;
            int mapY = (int)floorY;

            int texX = (int)(ctx->texture_width * (floorX - mapX)) & (ctx->texture_width - 1);
            int texY = (int)(ctx->texture_height * (floorY - mapY)) & (ctx->texture_height - 1);

            floorX += stepX;
            floorY += stepY;

            SDL_Rect srcRect = { texX, texY, 1, 1 };
            SDL_Rect dstFloor = { x, y, 1, 1 };
            SDL_Rect dstCeiling = { x, ctx->screen_height - y - 1, 1, 1 };

            // Draw floor texture
            SDL_RenderCopy(renderer, ctx->textures[ctx->floor_texture_index], &srcRect, &dstFloor);
            // Draw ceiling texture
            SDL_RenderCopy(renderer, ctx->textures[ctx->ceiling_texture_index], &srcRect, &dstCeiling);
        }
    }
}
