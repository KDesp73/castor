#include "screens.h"
#include "settings.h"
#include "ui.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>

static size_t buttonsWidth = 120;
static size_t buttonsHeight = 40;

#define BUTTON_DEFAULTS \
    .w = buttonsWidth, \
    .h = buttonsHeight, \
    .color = UI_COLOR_RED, \
    .default_color = UI_COLOR_RED, \
    .onhover = UI_COLOR_MAROON, \
    .font = ui->font

#define SCREEN_HEADER \
    SDL_ShowCursor(SDL_TRUE); \
    SDL_SetRelativeMouseMode(SDL_FALSE); \
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); \
    SDL_RenderClear(renderer)

#define SCREEN_TITLE_HEIGHT(screen_height) (screen_height * 0.2)
#define FIRST_BUTTON_HEIGHT(screen_height) (screen_height * 0.35)
#define NEXT_BUTTON_HEIGHT(screen_height, button_height, padding, index) \
    (FIRST_BUTTON_HEIGHT(screen_height) + (button_height + padding) * index)


int StartScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui)
{
    SCREEN_HEADER;

    UIButton start_button = {
        BUTTON_DEFAULTS,
        .label = "Start",
    };
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

    UIButtonOnHover(evt, &start_button);
    UIDrawButton(renderer, &start_button);
    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);

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
    SCREEN_HEADER;

    const char* title = "Paused";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

    size_t screen_height = ui->ctx->screen_height;

    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UIDrawText(renderer, title, (ui->ctx->screen_width - text_w) / 2, screen_height * 0.2, &title_font);

    size_t centerX = (ui->ctx->screen_width - buttonsWidth) / 2;

    size_t padding = 20;
    size_t btn_index = 0;
    UIButton resumeButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_HEIGHT(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Resume"
    };
    UIButtonOnHover(evt, &resumeButton);
    UIDrawButton(renderer, &resumeButton);

    UIButton settingsButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_HEIGHT(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Settings"
    };
    UIButtonOnHover(evt, &settingsButton);
    UIDrawButton(renderer, &settingsButton);

    UIButton exitButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_HEIGHT(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Exit"
    };
    UIButtonOnHover(evt, &exitButton);
    UIDrawButton(renderer, &exitButton);

    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);

    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) return -1;
        if(UIIsButtonPressed(evt, &exitButton)) return -1;
        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE) return 0;
        if(UIIsButtonPressed(evt, &resumeButton)) return 0;
        if(UIIsButtonPressed(evt, &settingsButton)) {
            int code = UIPollScreen(SettingsScreen, renderer, evt, ui);
            if(code == -1) return -1;
        }
    }

    return 69;
}

int SettingsScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui)
{
    SCREEN_HEADER;

    const char* title = "Settings";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UIDrawText(renderer, title, (ui->ctx->screen_width - text_w) / 2, SCREEN_TITLE_HEIGHT(ui->ctx->screen_height), &title_font);

    size_t centerX = (ui->ctx->screen_width - buttonsWidth) / 2;

    size_t padding = 20;
    size_t btn_index = 0;
    UIButton fullscreenButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_HEIGHT(ui->ctx->screen_height, buttonsHeight, padding, btn_index++),
        .label = (ui->ctx->fullscreen) ? "Window" : "Fullscreen"
    };
    UIButtonOnHover(evt, &fullscreenButton);
    UIDrawButton(renderer, &fullscreenButton);

    UIButton backButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_HEIGHT(ui->ctx->screen_height, buttonsHeight, padding, btn_index++),
        .label = "Back"
    };
    UIButtonOnHover(evt, &backButton);
    UIDrawButton(renderer, &backButton);

    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);

    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) return -1;
        if(UIIsButtonPressed(evt, &backButton)) return 0;
        if(UIIsButtonPressed(evt, &fullscreenButton)) {
            SetFullscreen(ui->ctx, !ui->ctx->fullscreen);
        }
    }

    return 69;
}
