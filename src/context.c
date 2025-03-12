#include "context.h"
#include "map.h"
#include "textures.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string.h>

bool ConstructRenderer(Context* ctx)
{
    ctx->window = SDL_CreateWindow(
            ctx->game_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ctx->screen_width, ctx->screen_height, SDL_WINDOW_SHOWN);

    if(!ctx->window) {
        fprintf(stderr, "Could not create window\n");
        return false;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);

    if(!ctx->renderer) {
        fprintf(stderr, "Could not create renderer\n");
        return false;
    }

    return true;
}

void ContextInit(Context* ctx)
{
    ctx->running = true;
    ctx->fov = 60;
    ctx->screen_width = DEFAULT_SCREEN_WIDTH;
    ctx->screen_height = DEFAULT_SCREEN_HEIGHT;
    ctx->texture_width = 64;
    ctx->texture_height = 64;
    ctx->mouse_sensitivity = 0.25;
    ctx->mouse_inverted = true;
    ctx->render_distance = 20.0f;

    ctx->enable_fog = true;
    ctx->fog_distance = 10;
}

void ContextFree(Context* ctx)
{
    if (ctx) {
        PlayerFree(&ctx->player);
        FreeTextures(ctx);
        MapFree(ctx->map, ctx->map_height);

        if (ctx->renderer) {
            SDL_DestroyRenderer(ctx->renderer);
            ctx->renderer = NULL;
        }

        if (ctx->window) {
            SDL_DestroyWindow(ctx->window);
            ctx->window = NULL;
        }
    }
}

void FreeTextures(Context* ctx)
{
    for(size_t i = 0; i < MAX_TEXTURES; i++) {
        if(ctx->textures[i] != NULL) {
            SDL_DestroyTexture(ctx->textures[i]); 
            ctx->textures[i] = NULL;
        }
        if(ctx->sprite_textures[i] != NULL) {
            SDL_DestroyTexture(ctx->sprite_textures[i]); 
            ctx->sprite_textures[i] = NULL;
        }
    }
    ctx->textures_loaded = false;
    ctx->sprites_loaded = false;
}

void LoadLevelMap(Context* ctx, const char* path)
{
    ctx->map = MapLoad(&ctx->map_height, &ctx->map_width, path);
}

void LoadTextures(Context* ctx)
{
    TexturesLoad(ctx->renderer, ctx->textures, TEXTURES_LIST_FILE);
    ctx->textures_loaded = true;
    TexturesLoad(ctx->renderer, ctx->sprite_textures, SPRITES_LIST_FILE);
    ctx->sprites_loaded = true;
}

void AppendSprite(Context* ctx, Sprite sprite)
{
    if(ctx->sprite_count >= MAX_TEXTURES) return;

    ctx->sprites[ctx->sprite_count++] = sprite;
}
