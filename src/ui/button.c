#include "ui.h"

void UIDrawButton(SDL_Renderer* renderer, UIButton* btn)
{
    SDL_Rect button = {btn->x, btn->y, btn->w, btn->h};
    SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(btn->color));
    SDL_RenderFillRect(renderer, &button);
    int text_w, text_h;
    TTF_SizeText(btn->font->ttf, btn->label, &text_w, &text_h);
    UIDrawText(renderer, btn->label, btn->x + (btn->w - text_w) / 2, btn->y + (btn->h - text_h) / 2, btn->font);
}

int UIIsButtonPressed(SDL_Event *event, UIButton* btn)
{
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        return UIIsButtonHovered(event, btn);
    }
    return 0;
}

int UIIsButtonHovered(SDL_Event *event, UIButton* btn)
{
    int mx = event->button.x;
    int my = event->button.y;
    return (mx >= btn->x && mx <= btn->x + btn->w && my >= btn->y && my <= btn->y + btn->h);
}
