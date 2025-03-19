#include "settings.h"
#include "context.h"
#include "textures.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

void SetFullscreen(Context* ctx, bool fullscreen)
{
    if (!ctx || !ctx->window || !ctx->renderer) {
        fprintf(stderr, "Error: Invalid context, window, or renderer!\n");
        return;
    }

    if (fullscreen) {
        SDL_DisplayMode displayMode;
        if (SDL_GetDesktopDisplayMode(0, &displayMode) == 0) {
            ctx->screen_width = displayMode.w;
            ctx->screen_height = displayMode.h;
            printf("Entering fullscreen mode: %d x %d\n", ctx->screen_width, ctx->screen_height);
        }
        SDL_SetWindowFullscreen(ctx->window, SDL_WINDOW_FULLSCREEN);
    } else {
        printf("Exiting fullscreen mode\n");
        ctx->screen_width = DEFAULT_SCREEN_WIDTH;
        ctx->screen_height = DEFAULT_SCREEN_HEIGHT;
        SDL_SetWindowFullscreen(ctx->window, 0);
        SDL_SetWindowSize(ctx->window, ctx->screen_width, ctx->screen_height);
    }

    // Let SDL process events to avoid race condition
    SDL_PumpEvents();
    SDL_Delay(50); 

    // Clear and destroy old renderer
    SDL_RenderClear(ctx->renderer);
    SDL_RenderPresent(ctx->renderer);
    SDL_DestroyRenderer(ctx->renderer);
    ctx->renderer = NULL;

    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ctx->renderer) {
        fprintf(stderr, "Error recreating renderer: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_RenderSetLogicalSize(ctx->renderer, ctx->screen_width, ctx->screen_height);

    ctx->fullscreen = fullscreen;

    LoadTextures(ctx);

    printf("Fullscreen toggle complete.\n");
}
