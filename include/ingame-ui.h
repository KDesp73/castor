#ifndef IN_GAME_UI_H
#define IN_GAME_UI_H
#include <SDL2/SDL_render.h>

static inline void RenderCrosshair(SDL_Renderer* renderer, int screen_width, int screen_height)
{
    int centerX = screen_width / 2;
    int centerY = screen_height / 2;
    int size = 15;
    int gap = 6;
    int thickness = 4;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, centerX - size, centerY + i, centerX - gap, centerY + i);
        SDL_RenderDrawLine(renderer, centerX + gap, centerY + i, centerX + size, centerY + i);
    }

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, centerX + i, centerY - size, centerX + i, centerY - gap);
        SDL_RenderDrawLine(renderer, centerX + i, centerY + gap, centerX + i, centerY + size);
    }
}

#endif // IN_GAME_UI_H
