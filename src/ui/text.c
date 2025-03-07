#include "ui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void UIFontOpen(UIFont* font, const char* path, size_t size, SDL_Color color)
{
    font->ttf = TTF_OpenFont(path, size);
    font->size = size;
    font->color = color;
}

void UIFontClose(UIFont *font) {
    if (font && font->ttf) {
        TTF_CloseFont(font->ttf);
        font->ttf = NULL;
    }
}

void UIDrawText(SDL_Renderer *renderer, const char *text, int x, int y, UIFont *font) {
    if (!renderer || !text || !font || !font->ttf) {
        printf("Error: Invalid arguments passed to UIDrawText\n");
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font->ttf, text, font->color);
    if (!surface) {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        printf("Error creating text texture: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h); // Ensure correct size

    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);  // Free surface immediately after creating texture
    SDL_DestroyTexture(texture);  // Destroy texture after rendering
}
