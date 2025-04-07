#include "renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string.h>

// TODO: Proper method definitions

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} SDLRenderData;

static bool sdl_init(castor_RenderContext* ctx, int width, int height, const char* title, bool fullscreen) {
    SDLRenderData* data = malloc(sizeof(SDLRenderData));
    if (!data) return false;

    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    data->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!data->window) return false;

    data->renderer = SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED);
    if (!data->renderer) return false;

    ctx->user_data = data;
    return true;
}

static void sdl_shutdown(castor_RenderContext* ctx) {
    SDLRenderData* data = ctx->user_data;
    if (data) {
        if (data->renderer) SDL_DestroyRenderer(data->renderer);
        if (data->window) SDL_DestroyWindow(data->window);
        free(data);
        ctx->user_data = NULL;
    }
}

static void sdl_clear(castor_RenderContext* ctx) {
    SDLRenderData* data = ctx->user_data;
    SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
    SDL_RenderClear(data->renderer);
}

static void sdl_present(castor_RenderContext* ctx) {
    SDLRenderData* data = ctx->user_data;
    SDL_RenderPresent(data->renderer);
}

static void* sdl_load_texture(castor_RenderContext* ctx, const char* path) {
    SDLRenderData* data = ctx->user_data;
    return IMG_LoadTexture(data->renderer, path);
}

static void sdl_destroy_texture(castor_RenderContext* ctx, void* texture) {
    SDL_DestroyTexture((SDL_Texture*)texture);
}

static void sdl_draw_texture(castor_RenderContext* ctx, void* texture, int x, int y, int w, int h) {
    SDLRenderData* data = ctx->user_data;
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(data->renderer, (SDL_Texture*)texture, NULL, &dst);
}

static void sdl_set_fullscreen(castor_RenderContext* ctx, bool enabled) {
    SDLRenderData* data = ctx->user_data;
    SDL_SetWindowFullscreen(data->window, enabled ? SDL_WINDOW_FULLSCREEN : 0);
}

static void sdl_resize(castor_RenderContext* ctx, int width, int height) {
    SDLRenderData* data = ctx->user_data;
    SDL_SetWindowSize(data->window, width, height);
}

castor_RenderAPI create_sdl_render_api() {
    castor_RenderAPI api = {
        .init = sdl_init,
        .shutdown = sdl_shutdown,
        .clear = sdl_clear,
        .present = sdl_present,
        .draw_texture = sdl_draw_texture,
        .load_texture = sdl_load_texture,
        .destroy_texture = sdl_destroy_texture,
        .set_fullscreen = sdl_set_fullscreen,
        .resize = sdl_resize,
    };
    return api;
}

