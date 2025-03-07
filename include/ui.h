#ifndef UI_H
#define UI_H

#include "context.h"
#include "engine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

/* COLORS */
#define UI_COLOR_WHITE       (SDL_Color) {255,255,255,255}
#define UI_COLOR_BLACK       (SDL_Color) {0,0,0,255}
#define UI_COLOR_RED         (SDL_Color) {255, 0, 0, 255}
#define UI_COLOR_GREEN       (SDL_Color) {0, 255, 0, 255}
#define UI_COLOR_BLUE        (SDL_Color) {0, 0, 255, 255}
#define UI_COLOR_YELLOW      (SDL_Color) {255, 255, 0, 255}
#define UI_COLOR_CYAN        (SDL_Color) {0, 255, 255, 255}
#define UI_COLOR_MAGENTA     (SDL_Color) {255, 0, 255, 255}
#define UI_COLOR_ORANGE      (SDL_Color) {255, 165, 0, 255}
#define UI_COLOR_PURPLE      (SDL_Color) {128, 0, 128, 255}
#define UI_COLOR_PINK        (SDL_Color) {255, 105, 180, 255}
#define UI_COLOR_LIME        (SDL_Color) {50, 205, 50, 255}
#define UI_COLOR_GOLD        (SDL_Color) {255, 215, 0, 255}
#define UI_COLOR_SILVER      (SDL_Color) {192, 192, 192, 255}
#define UI_COLOR_BRONZE      (SDL_Color) {205, 127, 50, 255}
#define UI_COLOR_TURQUOISE   (SDL_Color) {64, 224, 208, 255}
#define UI_COLOR_NAVY        (SDL_Color) {0, 0, 128, 255}
#define UI_COLOR_MAROON      (SDL_Color) {128, 0, 0, 255}
#define UI_COLOR_TEAL        (SDL_Color) {0, 128, 128, 255}
#define UI_COLOR_OLIVE       (SDL_Color) {128, 128, 0, 255}
#define UI_COLOR_DARK_GRAY   (SDL_Color) {64, 64, 64, 255}
#define UI_COLOR_GRAY        (SDL_Color) {128, 128, 128, 255}
#define UI_COLOR_LIGHT_GRAY  (SDL_Color) {192, 192, 192, 255}
#define UI_COLOR_TRANSPARENT (SDL_Color) {0, 0, 0, 0}  // Fully transparent

#define UI_COLOR_PARAMS(color) color.r, color.g, color.b, color.a

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
    SDL_Color color;
    SDL_Color default_color;
    SDL_Color onhover;
} UIButton;
void UIDrawButton(SDL_Renderer* renderer, UIButton* btn);
int UIIsButtonPressed(SDL_Event *event, UIButton* btn);
int UIIsButtonHovered(SDL_Event *event, UIButton* btn);
void UIButtonOnHover(SDL_Event* event, UIButton* btn);


/* UI */
typedef struct {
    Context* ctx;
    UIFont* font;
} UI;
void UIOpen(UI* ui, Context* ctx, UIFont* font);
void UIClose(UI* ui);

/* SCREENS */
/*
 * When a screen returns 0 it means that it's completed its purpose and we continue where we left of.
 * In case it returns -1 it means that the user tried to quit the game.
 * Any other code is irrelevant
 */
typedef int (*UIScreen)(SDL_Renderer*, SDL_Event*, UI*) ;

static inline int UIPollScreen(UIScreen screen, SDL_Renderer* renderer, SDL_Event* event, UI* ui)
{
    int code = screen(renderer, event, ui);
    while (code != 0) {
        if (code == -1) {
            return -1; // Exit early if screen function returns -1
        }
        SDL_Delay(16); // Prevent CPU overuse
        code = screen(renderer, event, ui); // Call the screen function again
    }
    return 0;
}
#define UI_POLL_SCREEN UIPollScreen

/* TOAST */
typedef struct {
    char message[256];
    int duration_ms;
    Uint32 start_time;
    bool active;
} UIToast;

void UIShowToast(UIToast *toast, const char *message, int duration_ms);
void UIRenderToast(SDL_Renderer *renderer, TTF_Font *font, UIToast *toast, int screen_width, int screen_height);

#endif // UI_H
