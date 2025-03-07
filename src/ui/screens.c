#include "ui.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>

static UIButton start_button = {
    .w = 100,
    .h = 40,
    .label = "Start",
    .color = UI_COLOR_RED 
};

int StartScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui)
{
    start_button.font = ui->font;
    start_button.x = (ui->ctx->screen_width - start_button.w) / 2;
    start_button.y = (ui->ctx->screen_height - start_button.h) / 2 + 100;

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    const char* title = ui->ctx->game_name;
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 80, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UIDrawText(renderer, title, (ui->ctx->screen_width - text_w) / 2, (ui->ctx->screen_height - text_h) / 2 - 100, &title_font);
    UIFontClose(&title_font);


    if (UIIsButtonHovered(evt, &start_button)) {
        start_button.color = UI_COLOR_MAROON;
    } else {
        start_button.color = UI_COLOR_RED;
    }

    UIDrawButton(renderer, &start_button);
    SDL_RenderPresent(renderer);

    if(UIIsButtonPressed(evt, &start_button))
    {
        return 0;
    }

    return -1;
}

int PauseScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui)
{
    SDL_ShowCursor(SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_FALSE);

    while (1) {  
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        const char* title = "Paused";
        UIFont title_font = {0};
        UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

        int text_w, text_h;
        TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
        UIDrawText(renderer, title, (ui->ctx->screen_width - text_w) / 2, (ui->ctx->screen_height - text_h) / 2 - 200, &title_font);

        UIFontClose(&title_font);
        SDL_RenderPresent(renderer);

        // Wait for ESC key press to exit pause screen
        while (SDL_PollEvent(evt)) {
            if (evt->type == SDL_QUIT) {
                return -1;  // Quit game
            }
            if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                return 0;  // Exit pause screen
            }
        }

        SDL_Delay(16);  // Prevent CPU overuse
    }
}

