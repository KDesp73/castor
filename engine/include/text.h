#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#define COLOR_WHITE (SDL_Color) {255,255,255,255}
#define COLOR_RED (SDL_Color) {255,0,0,255}
#define COLOR_GREEN (SDL_Color) {0,255,0,255}
#define COLOR_BLUE (SDL_Color) {0,0,255,255}
#define COLOR_BLACK (SDL_Color) {0,0,0,255}

void castor_RenderText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, int x, int y, const char *fmt, ...);

#endif // TEXT_H
