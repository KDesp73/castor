#include "core.h"

void castor_LoadTextures(castor_Context* ctx)
{
    castor_TexturesLoad(ctx->sdl.renderer, ctx->raycaster.textures, TEXTURES_LIST_FILE);
    ctx->raycaster.textures_loaded = true;
}
void castor_LoadSprites(castor_Context* ctx)
{
    castor_TexturesLoad(ctx->sdl.renderer, ctx->raycaster.sprite_textures, SPRITES_LIST_FILE);
    ctx->raycaster.sprites_loaded = true;
}

void castor_FreeTextures(castor_Context* ctx)
{
    for(size_t i = 0; i < MAX_TEXTURES; i++) {
        if(ctx->raycaster.textures[i] != NULL) {
            SDL_DestroyTexture(ctx->raycaster.textures[i]); 
            ctx->raycaster.textures[i] = NULL;
        }
    }
    ctx->raycaster.textures_loaded = false;
    ctx->raycaster.sprites_loaded = false;
}

bool castor_ConstructRenderer(castor_Context* ctx)
{
    ctx->sdl.window = SDL_CreateWindow(
            ctx->game_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ctx->sdl.screen_width, ctx->sdl.screen_height, SDL_WINDOW_SHOWN);

    if(!ctx->sdl.window) {
        fprintf(stderr, "Could not create window\n");
        return false;
    }

    ctx->sdl.renderer = SDL_CreateRenderer(ctx->sdl.window, -1, SDL_RENDERER_ACCELERATED);

    if(!ctx->sdl.renderer) {
        fprintf(stderr, "Could not create renderer\n");
        return false;
    }

    return true;
}

