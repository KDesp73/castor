#ifndef IN_GAME_UI_H
#define IN_GAME_UI_H
#include <SDL2/SDL_render.h>

static inline void RenderCrosshair(SDL_Renderer* renderer, int screen_width, int screen_height)
{
    int centerX = screen_width / 2;
    int centerY = screen_height / 2;
    int size = 15;   // Half the length of each crosshair line (increased)
    int gap = 6;     // Gap in the middle (slightly bigger)
    int thickness = 4; // Increased thickness

    // Set crosshair color (yellow)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // Draw horizontal lines (left and right, with gap)
    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, centerX - size, centerY + i, centerX - gap, centerY + i);
        SDL_RenderDrawLine(renderer, centerX + gap, centerY + i, centerX + size, centerY + i);
    }

    // Draw vertical lines (top and bottom, with gap)
    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, centerX + i, centerY - size, centerX + i, centerY - gap);
        SDL_RenderDrawLine(renderer, centerX + i, centerY + gap, centerX + i, centerY + size);
    }
}

#endif // IN_GAME_UI_H
