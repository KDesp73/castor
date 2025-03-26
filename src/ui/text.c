#include "ui.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

void UIFontOpen(UIFont* font, const char* path, size_t size, SDL_Color color)
{
    font->ttf = TTF_OpenFont(path, size);
    font->size = size;
    font->color = color;
}

void UIFontClose(UIFont *font)
{
    if (font && font->ttf) {
        TTF_CloseFont(font->ttf);
        font->ttf = NULL;
    }
}

void UITextRender(SDL_Renderer *renderer, const char *text, int x, int y, UIFont *font)
{
    if (!renderer || !text || !font || !font->ttf) {
        printf("Error: Invalid arguments passed to UIDrawText\n");
        return;
    }

    TTF_SetFontHinting(font->ttf, TTF_HINTING_NORMAL);  // Use normal hinting

    // Use blended rendering for anti-aliasing
    SDL_Surface *surface = TTF_RenderText_Blended(font->ttf, text, font->color);
    if (!surface) {
        printf("Error creating text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Error creating text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
