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

#define BUTTON_DEFAULTS(ctx) \
    .w = buttonsWidth, \
    .h = buttonsHeight, \
    .color = UI_COLOR_RED, \
    .default_color = UI_COLOR_RED, \
    .disabled_color = UI_COLOR_GRAY, \
    .onhover = UI_COLOR_MAROON, \
    .font = ctx->ui.font

#define SCREEN_HEADER(ctx) \
    SDL_ShowCursor(SDL_TRUE); \
    SDL_SetRelativeMouseMode(SDL_FALSE); \
    SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255); \
    SDL_RenderClear(ctx->renderer)

#define SCREEN_TITLE_Y(screen_height) (screen_height * 0.2)
#define FIRST_BUTTON_Y(screen_height) (screen_height * 0.35)
#define NEXT_BUTTON_Y(screen_height, button_height, padding, index) \
    (FIRST_BUTTON_Y(screen_height) + (button_height + padding) * index)


int StartScreen(void* context, SDL_Event* evt)
{
    Context* ctx = (Context*) context;
    SCREEN_HEADER(ctx);

    UIButton start_button = {
        BUTTON_DEFAULTS(ctx),
        .label = "Start",
    };
    start_button.x = (ctx->screen_width - start_button.w) / 2;
    start_button.y = (ctx->screen_height - start_button.h) / 2 + 100;


    SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->renderer);

    const char* title = ctx->game_name;
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 80, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UITextRender(ctx->renderer, title, (ctx->screen_width - text_w) / 2, (ctx->screen_height - text_h) / 2 - 100, &title_font);

    UIButtonOnHover(evt, &start_button);
    UIButtonRender(ctx->renderer, &start_button);
    SDL_RenderPresent(ctx->renderer);
    UIFontClose(&title_font);

    // Poll events here
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            return -1;
        }

        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_RETURN) return 0;
        if (UIButtonIsPressed(evt, &start_button)) {
            return 0;
        }

    }

    return 69;
}

int PauseScreen(void* context, SDL_Event* evt)
{
    Context* ctx = context;
    SCREEN_HEADER(ctx);

    const char* title = "Paused";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

    size_t screen_height = ctx->screen_height;

    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UITextRender(ctx->renderer, title, (ctx->screen_width - text_w) / 2, screen_height * 0.2, &title_font);

    size_t centerX = (ctx->screen_width - buttonsWidth) / 2;

    size_t padding = 20;
    size_t btn_index = 0;
    UIButton resumeButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Resume"
    };
    UIButtonOnHover(evt, &resumeButton);
    UIButtonRender(ctx->renderer, &resumeButton);

    UIButton settingsButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Settings"
    };
    UIButtonOnHover(evt, &settingsButton);
    UIButtonRender(ctx->renderer, &settingsButton);

    UIButton exitButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Exit"
    };
    UIButtonOnHover(evt, &exitButton);
    UIButtonRender(ctx->renderer, &exitButton);

    SDL_RenderPresent(ctx->renderer);
    UIFontClose(&title_font);

    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) return -1;
        if(UIButtonIsPressed(evt, &exitButton)) return -1;
        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE) return 0;
        if(UIButtonIsPressed(evt, &resumeButton)) return 0;
        if(UIButtonIsPressed(evt, &settingsButton)) {
            int code = UIPollScreen(SettingsScreen, ctx, evt);
            if(code == -1) return -1;
        }
    }

    return 69;
}

int SettingsScreen(void* context, SDL_Event* evt)
{
    Context* ctx = context;
    SCREEN_HEADER(ctx);
    const char* title = "Settings";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    
    size_t centerX = (ctx->screen_width - buttonsWidth) / 2;
    size_t padding = 20;
    size_t btn_index = 0;
    UIButton fullscreenButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(ctx->screen_height, buttonsHeight, padding, btn_index++),
        .label = (ctx->fullscreen) ? "Window" : "Fullscreen",
    };
    // NOTE: Fullscreen button disabled because of known SDL bug
    fullscreenButton.disabled = true;
    fullscreenButton.color = fullscreenButton.disabled_color;
    
    static float sensitivity = 30;
    UISlider sensitivitySlider = {0};
    UISliderInit(&sensitivitySlider, 
            (ctx->screen_width - buttonsWidth) / 2,
            (fullscreenButton.y + fullscreenButton.h + padding) + padding,
            buttonsWidth, 15, 0, 100, sensitivity, 
            UI_COLOR_GRAY, UI_COLOR_RED);
    sensitivitySlider.dragging = false; // Explicitly initialize dragging state

    static float render_distance = 20;
    UISlider renderDistanceSlider = {0};
    UISliderInit(&renderDistanceSlider, 
            (ctx->screen_width - buttonsWidth) / 2,
            sensitivitySlider.y + fullscreenButton.h + padding,
            buttonsWidth, 15, 10, 50, render_distance, 
            UI_COLOR_GRAY, UI_COLOR_RED);
    sensitivitySlider.dragging = false; // Explicitly initialize dragging state

    
    btn_index++;
    btn_index++;
    UIButton backButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(ctx->screen_height, buttonsHeight, padding, btn_index++),
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
        UISliderHandleEvent(&renderDistanceSlider, evt);
        
        if (UIButtonIsPressed(evt, &backButton)) {
            result = 0;
            break; // Exit the event loop immediately
        }
        
        if (UIButtonIsPressed(evt, &fullscreenButton)) {
            SetFullscreen(ctx, !ctx->fullscreen);
            fullscreenButton.label = (ctx->fullscreen) ? "Window" : "Fullscreen";
        }
    }
    
    sensitivity = sensitivitySlider.value;
    ctx->mouse_sensitivity = sensitivity / 100;

    render_distance = renderDistanceSlider.value;
    ctx->render_distance = render_distance;
    
    // Rendering starts here
    SDL_SetRenderDrawColor(ctx->renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->renderer);
    
    UITextRender(ctx->renderer, title, (ctx->screen_width - text_w) / 2, SCREEN_TITLE_Y(ctx->screen_height), &title_font);
    UIButtonOnHover(evt, &fullscreenButton);
    UIButtonRender(ctx->renderer, &fullscreenButton);
    UIButtonOnHover(evt, &backButton);
    UIButtonRender(ctx->renderer, &backButton);

    UITextRender(ctx->renderer, "Sensitivity", sensitivitySlider.x, sensitivitySlider.y-sensitivitySlider.h - ctx->ui.font->size/2, ctx->ui.font);
    UISliderRender(ctx->renderer, &sensitivitySlider);
    UITextRender(ctx->renderer, "Render Distance", renderDistanceSlider.x, renderDistanceSlider.y-renderDistanceSlider.h - ctx->ui.font->size/2, ctx->ui.font);
    UISliderRender(ctx->renderer, &renderDistanceSlider);
    
    SDL_RenderPresent(ctx->renderer);
    UIFontClose(&title_font);
    
    return result;
}
