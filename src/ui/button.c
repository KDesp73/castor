#include "ui.h"

static void drawButton(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect button = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Blue color
    SDL_RenderFillRect(renderer, &button);
}

void UIDrawButton(SDL_Renderer* renderer, UIButton* btn)
{
    drawButton(renderer, btn->x, btn->y, btn->w, btn->h);
    int text_w, text_h;
    TTF_SizeText(btn->font->ttf, btn->label, &text_w, &text_h);
    UIDrawText(renderer, btn->label, btn->x + (btn->w - text_w) / 2, btn->y + (btn->h - text_h) / 2, btn->font);
}

int UIIsButtonPressed(SDL_Event *event, UIButton* btn)
{
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mx = event->button.x;
        int my = event->button.y;
        return (mx >= btn->x && mx <= btn->x + btn->w && my >= btn->y && my <= btn->y + btn->h);
    }
    return 0;
}
