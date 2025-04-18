#ifndef CASTOR_UI_H
#define CASTOR_UI_H

#include "world.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

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

typedef struct {
    TTF_Font* ttf;
    size_t size;
    SDL_Color color;
} UIFont;
void UIFontOpen(UIFont* font, const char* path, size_t size, SDL_Color color);
void UIFontClose(UIFont* font);
void UITextRender(SDL_Renderer *renderer, const char *text, int x, int y, UIFont* font);

/* BUTTON */
typedef struct {
    int x, y, w, h;
    char* label;
    UIFont* font;
    SDL_Color color;
    SDL_Color default_color;
    SDL_Color disabled_color;
    SDL_Color onhover;
    bool disabled;
} UIButton;
void UIButtonRender(SDL_Renderer* renderer, UIButton* btn);
int UIButtonIsPressed(SDL_Event *event, UIButton* btn);
int UIButtonIsHovered(SDL_Event *event, UIButton* btn);
void UIButtonOnHover(SDL_Event* event, UIButton* btn);

/* TOAST */
typedef struct {
    char message[256];
    Uint32 duration;
    Uint32 start_time;
    bool active;
    int x, y;
} UIToast;

void UIToastInit(UIToast *toast, const char *message, int duration_ms, int x, int y);
void UIToastRender(SDL_Renderer *renderer, UIFont *font, UIToast *toast, int screen_width, int screen_height);

/* SLIDER */
typedef struct {
    int x, y, w, h;          // Position and size
    float value;             // Current slider value (0.0 - 1.0 normalized)
    float min, max;          // Min and max value range
    SDL_Color barColor;      // Slider bar color
    SDL_Color knobColor;     // Knob color
    bool dragging;           // Whether the user is currently dragging the knob
} UISlider;

void UISliderInit(UISlider *slider, int x, int y, int w, int h, float min, float max, float initial, SDL_Color barColor, SDL_Color knobColor);
void UISliderRender(SDL_Renderer *renderer, UISlider *slider);
void UISliderHandleEvent(UISlider *slider, SDL_Event *event);

/* UI */
#define MAX_TOASTS 16
typedef struct {
    UIFont* font;

    // In-game ui elements
    UIToast toasts[MAX_TOASTS];
    size_t toast_count;
    #define MAX_DAMAGE_NUMBERS 16
    castor_DamageNumber damage_numbers[MAX_DAMAGE_NUMBERS];
    size_t damage_number_count;
} UI;
void UIOpen(UI* ui, UIFont* font);
void UIRender(UI* ui, void* context);
void UIClose(UI* ui);

void UIAppendToast(UI* ui, UIToast toast);

/* SCREENS */
/*
 * When a screen returns 0 it means that it's completed its purpose and we continue where we left of.
 * In case it returns -1 it means that the user tried to quit the game.
 * Any other code is irrelevant
 */
typedef int (*UIScreen)(void*, SDL_Event*) ;

static inline int UIPollScreen(UIScreen screen, void* ctx, SDL_Event* event)
{
    int code = screen(ctx, event);
    while (code != 0) {
        if (code == -1) {
            return -1; // Exit early if screen function returns -1
        }
        SDL_Delay(16); // Prevent CPU overuse
        code = screen(ctx, event); // Call the screen function again
    }
    return 0;
}
#define UI_POLL_SCREEN UIPollScreen

#endif // CASTOR_UI_H
