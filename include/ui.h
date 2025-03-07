#ifndef UI_H
#define UI_H

#include "context.h"
#include "engine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

/* COLORS */
#define UI_COLOR_WHITE (SDL_Color) {255,255,255,255}
#define UI_COLOR_BLACK (SDL_Color) {0,0,0,255}

/* TEXT */
#define UI_GLOBAL_FONT "./assets/fonts/FreeMonoBold.ttf"

typedef struct {
    TTF_Font* ttf;
    size_t size;
    SDL_Color color;
} UIFont;
void UIFontOpen(UIFont* font, const char* path, size_t size, SDL_Color color);
void UIFontClose(UIFont* font);
void UIDrawText(SDL_Renderer *renderer, const char *text, int x, int y, UIFont* font);

/* BUTTON */
typedef struct {
    int x, y, w, h;
    char label[64];
    UIFont* font;
} UIButton;
void UIDrawButton(SDL_Renderer* renderer, UIButton* btn);
int UIIsButtonPressed(SDL_Event *event, UIButton* btn);

/* UI */
typedef struct {
    const Context* ctx;
    UIFont* font;
} UI;
void UIOpen(UI* ui, Context* ctx, UIFont* font);
void UIClose(UI* ui);

/* SCREENS */
typedef int (*UIScreen)(SDL_Renderer*, SDL_Event*, UI*) ;
int StartScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui);
int PauseScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui);

#define UI_POLL_SCREEN(screen, ctx, event) \
    while(screen != 0) { \
        FPS_START(ctx); \
        while (SDL_PollEvent(&event)) { \
            if (event.type == SDL_QUIT) { \
                return; \
            } \
        } \
        FPS_END(ctx); \
    }

#endif // UI_H
