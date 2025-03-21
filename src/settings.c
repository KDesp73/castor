#include "settings.h"
#include "context.h"
#include "textures.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

void SetFullscreen(Context* ctx, bool fullscreen)
{
    if (!ctx || !ctx->sdl.window || !ctx->sdl.renderer) {
        fprintf(stderr, "Error: Invalid context, window, or renderer!\n");
        return;
    }

    if (fullscreen) {
        SDL_DisplayMode displayMode;
        if (SDL_GetDesktopDisplayMode(0, &displayMode) == 0) {
            ctx->sdl.screen_width = displayMode.w;
            ctx->sdl.screen_height = displayMode.h;
            printf("Entering fullscreen mode: %d x %d\n", ctx->sdl.screen_width, ctx->sdl.screen_height);
        }
        SDL_SetWindowFullscreen(ctx->sdl.window, SDL_WINDOW_FULLSCREEN);
    } else {
        printf("Exiting fullscreen mode\n");
        ctx->sdl.screen_width = DEFAULT_SCREEN_WIDTH;
        ctx->sdl.screen_height = DEFAULT_SCREEN_HEIGHT;
        SDL_SetWindowFullscreen(ctx->sdl.window, 0);
        SDL_SetWindowSize(ctx->sdl.window, ctx->sdl.screen_width, ctx->sdl.screen_height);
    }

    // Let SDL process events to avoid race condition
    SDL_PumpEvents();
    SDL_Delay(50); 

    // Clear and destroy old renderer
    SDL_RenderClear(ctx->sdl.renderer);
    SDL_RenderPresent(ctx->sdl.renderer);
    SDL_DestroyRenderer(ctx->sdl.renderer);
    ctx->sdl.renderer = NULL;

    ctx->sdl.renderer = SDL_CreateRenderer(ctx->sdl.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ctx->sdl.renderer) {
        fprintf(stderr, "Error recreating renderer: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_RenderSetLogicalSize(ctx->sdl.renderer, ctx->sdl.screen_width, ctx->sdl.screen_height);

    ctx->settings.fullscreen = fullscreen;

    LoadTextures(ctx);

    printf("Fullscreen toggle complete.\n");
}
