#include "text.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>


void RenderText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, const char *fmt, ...)
{
    char text[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(text, sizeof(text), fmt, args);
    va_end(args);

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        fprintf(stderr, "Failed to create text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);

    if (!texture) {
        fprintf(stderr, "Failed to create text texture: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}
