#include "ui.h"

void UIButtonRender(SDL_Renderer* renderer, UIButton* btn)
{
    SDL_Rect button = {btn->x, btn->y, btn->w, btn->h};
    SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(btn->color));
    SDL_RenderFillRect(renderer, &button);
    int text_w, text_h;
    TTF_SizeText(btn->font->ttf, btn->label, &text_w, &text_h);
    UITextRender(renderer, btn->label, btn->x + (btn->w - text_w) / 2, btn->y + (btn->h - text_h) / 2, btn->font);
}

int UIButtonIsPressed(SDL_Event *event, UIButton* btn)
{
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        return UIButtonIsHovered(event, btn);
    }
    return 0;
}

int UIButtonIsHovered(SDL_Event *event, UIButton* btn)
{
    int mx = event->button.x;
    int my = event->button.y;
    return (mx >= btn->x && mx <= btn->x + btn->w && my >= btn->y && my <= btn->y + btn->h);
}

void UIButtonOnHover(SDL_Event* event, UIButton* btn)
{
    if (UIButtonIsHovered(event, btn)) {
        btn->color = btn->onhover; // Set hover color
    } else {
        btn->color = btn->default_color;
    }
}
