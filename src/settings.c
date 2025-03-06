#include "settings.h"
#include "context.h"
#include <SDL2/SDL_video.h>
#include <stdbool.h>

void SetFullscreen(Context* ctx)
{
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode) == 0) {
        int screenWidth = displayMode.w;
        int screenHeight = displayMode.h;
        printf("Screen resolution: %d x %d\n", screenWidth, screenHeight);
    }
    ctx->screen_width = displayMode.w;
    ctx->screen_height = displayMode.h;
    SDL_SetWindowFullscreen(ctx->window, SDL_WINDOW_FULLSCREEN);

}
