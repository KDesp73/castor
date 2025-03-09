#include "settings.h"
#include "context.h"
#include <SDL2/SDL_video.h>
#include <stdbool.h>

void SetFullscreen(Context* ctx, bool fullscreen)
{
    if (!ctx || !ctx->window) {
        fprintf(stderr, "Error: Invalid context or window!\n");
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

    ctx->fullscreen = !ctx->fullscreen;
}
