#include "screens.h"
#include "settings.h"
#include "ui.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

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

#define SCREEN_TITLE_Y(screen_height) (screen_height * 0.2)
#define FIRST_BUTTON_Y(screen_height) (screen_height * 0.35)
#define NEXT_BUTTON_Y(screen_height, button_height, padding, index) \
    (FIRST_BUTTON_Y(screen_height) + (button_height + padding) * index)


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
    UITextRender(renderer, title, (ui->ctx->screen_width - text_w) / 2, (ui->ctx->screen_height - text_h) / 2 - 100, &title_font);

    UIButtonOnHover(evt, &start_button);
    UIButtonRender(renderer, &start_button);
    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);

    // Poll events here
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            return -1;
        }

        if (UIButtonIsPressed(evt, &start_button)) {
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
    UITextRender(renderer, title, (ui->ctx->screen_width - text_w) / 2, screen_height * 0.2, &title_font);

    size_t centerX = (ui->ctx->screen_width - buttonsWidth) / 2;

    size_t padding = 20;
    size_t btn_index = 0;
    UIButton resumeButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Resume"
    };
    UIButtonOnHover(evt, &resumeButton);
    UIButtonRender(renderer, &resumeButton);

    UIButton settingsButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Settings"
    };
    UIButtonOnHover(evt, &settingsButton);
    UIButtonRender(renderer, &settingsButton);

    UIButton exitButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Exit"
    };
    UIButtonOnHover(evt, &exitButton);
    UIButtonRender(renderer, &exitButton);

    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);

    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) return -1;
        if(UIButtonIsPressed(evt, &exitButton)) return -1;
        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE) return 0;
        if(UIButtonIsPressed(evt, &resumeButton)) return 0;
        if(UIButtonIsPressed(evt, &settingsButton)) {
            int code = UIPollScreen(SettingsScreen, renderer, evt, ui);
            if(code == -1) return -1;
        }
    }

    return 69;
}

int SettingsScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui)
{
    SCREEN_HEADER; // Ensure this is defined elsewhere.
    const char* title = "Settings";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    
    size_t centerX = (ui->ctx->screen_width - buttonsWidth) / 2;
    size_t padding = 20;
    size_t btn_index = 0;
    UIButton fullscreenButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_Y(ui->ctx->screen_height, buttonsHeight, padding, btn_index++),
        .label = (ui->ctx->fullscreen) ? "Window" : "Fullscreen"
    };
    
    static float sensitivity = 30;
    UISlider sensitivitySlider = {0};
    size_t sliderWidth = 150;
    UISliderInit(&sensitivitySlider, 
            (ui->ctx->screen_width - sliderWidth) / 2,
            (fullscreenButton.y + fullscreenButton.h + padding) + padding,
            sliderWidth, 15, 0, 100, sensitivity, 
            UI_COLOR_GRAY, UI_COLOR_RED);
    sensitivitySlider.dragging = false; // Explicitly initialize dragging state
    
    btn_index++;
    UIButton backButton = {
        BUTTON_DEFAULTS,
        .x = centerX,
        .y = NEXT_BUTTON_Y(ui->ctx->screen_height, buttonsHeight, padding, btn_index++),
        .label = "Back"
    };
    
    int result = 69;
    
    // Handle all events
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            result = -1;
            break;
        }
        
        UISliderHandleEvent(&sensitivitySlider, evt);
        
        if (UIButtonIsPressed(evt, &backButton)) {
            result = 0;
            break; // Exit the event loop immediately
        }
        
        if (UIButtonIsPressed(evt, &fullscreenButton)) {
            SetFullscreen(ui->ctx, !ui->ctx->fullscreen);
            fullscreenButton.label = (ui->ctx->fullscreen) ? "Window" : "Fullscreen";
        }
    }
    
    sensitivity = sensitivitySlider.value;
    ui->ctx->mouse_sensitivity = sensitivity / 100;
    
    // Rendering starts here
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    
    UITextRender(renderer, title, (ui->ctx->screen_width - text_w) / 2, SCREEN_TITLE_Y(ui->ctx->screen_height), &title_font);
    UIButtonOnHover(evt, &fullscreenButton);
    UIButtonRender(renderer, &fullscreenButton);
    UIButtonOnHover(evt, &backButton);
    UIButtonRender(renderer, &backButton);

    UITextRender(renderer, "Sensitivity", sensitivitySlider.x, sensitivitySlider.y-sensitivitySlider.h - ui->font->size/2, ui->font);
    UISliderRender(renderer, &sensitivitySlider);
    
    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);
    
    return result;
}
