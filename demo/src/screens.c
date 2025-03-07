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
    .color = UI_COLOR_RED,
    .default_color = UI_COLOR_RED,
    .onhover = UI_COLOR_MAROON
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

    UIButtonOnHover(evt, &start_button);
    UIDrawButton(renderer, &start_button);
    SDL_RenderPresent(renderer);

    // Poll events here
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            return -1;
        }

        if (UIIsButtonPressed(evt, &start_button)) {
            return 0;
        }

    }

    return 69;
}

int PauseScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui)
{
    SDL_ShowCursor(SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    const char* title = "Paused";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UIDrawText(renderer, title, (ui->ctx->screen_width - text_w) / 2, (ui->ctx->screen_height - text_h) / 2 - 200, &title_font);

    size_t buttonsWidth = 100;
    size_t buttonsHeight = 50;
    UIFont buttonsFont = {0};
    UIFontOpen(&buttonsFont, UI_GLOBAL_FONT, 20, UI_COLOR_WHITE);
    size_t centerX = (ui->ctx->screen_width - buttonsWidth) / 2;

    UIButton resumeButton = {
        .color = UI_COLOR_RED,
        .default_color = UI_COLOR_RED,
        .onhover = UI_COLOR_MAROON,
        .w = buttonsWidth,
        .h = buttonsHeight,
        .x = centerX,
        .y = 400,
        .font = &buttonsFont,
        .label = "Resume"
    };

    UIButtonOnHover(evt, &resumeButton);
    UIDrawButton(renderer, &resumeButton);

    SDL_RenderPresent(renderer);
    UIFontClose(&buttonsFont);
    UIFontClose(&title_font);

    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            return -1;  // Quit game
        }
        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            return 0;  // Exit pause screen
        }
    }

    return 69;
}

