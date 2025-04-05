#include "screens.h"
#include "SDL2/SDL_ttf.h"
#include "ingame-ui.h"
#include "settings.h"
#include "ui.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>

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
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255); \
    SDL_RenderClear(ctx->sdl.renderer)

#define SCREEN_TITLE_Y(screen_height) (screen_height * 0.2)
#define FIRST_BUTTON_Y(screen_height) (screen_height * 0.35)
#define NEXT_BUTTON_Y(screen_height, button_height, padding, index) \
    (FIRST_BUTTON_Y(screen_height) + (button_height + padding) * index)


int StartScreen(void* context, SDL_Event* evt)
{
    castor_Context* ctx = (castor_Context*) context;
    SCREEN_HEADER(ctx);

    UIButton start_button = {
        BUTTON_DEFAULTS(ctx),
        .label = "Start",
    };
    start_button.x = (ctx->sdl.screen_width - start_button.w) / 2;
    start_button.y = (ctx->sdl.screen_height - start_button.h) / 2 + 100;


    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);

    const char* title = ctx->game_name;
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 80, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UITextRender(ctx->sdl.renderer, title, (ctx->sdl.screen_width - text_w) / 2, (ctx->sdl.screen_height - text_h) / 2 - 100, &title_font);

    UIButtonOnHover(evt, &start_button);
    UIButtonRender(ctx->sdl.renderer, &start_button);
    SDL_RenderPresent(ctx->sdl.renderer);
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
    castor_Context* ctx = context;
    SCREEN_HEADER(ctx);

    const char* title = "Paused";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

    size_t screen_height = ctx->sdl.screen_height;

    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    UITextRender(ctx->sdl.renderer, title, (ctx->sdl.screen_width - text_w) / 2, screen_height * 0.2, &title_font);

    size_t centerX = (ctx->sdl.screen_width - buttonsWidth) / 2;

    size_t padding = 20;
    size_t btn_index = 0;
    UIButton resumeButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Resume"
    };
    UIButtonOnHover(evt, &resumeButton);
    UIButtonRender(ctx->sdl.renderer, &resumeButton);

    UIButton settingsButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Settings"
    };
    UIButtonOnHover(evt, &settingsButton);
    UIButtonRender(ctx->sdl.renderer, &settingsButton);

    UIButton contorlsButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Controls"
    };
    UIButtonOnHover(evt, &contorlsButton);
    UIButtonRender(ctx->sdl.renderer, &contorlsButton);

    UIButton exitButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(screen_height, buttonsHeight, padding, btn_index++),
        .label = "Exit"
    };
    UIButtonOnHover(evt, &exitButton);
    UIButtonRender(ctx->sdl.renderer, &exitButton);

    SDL_RenderPresent(ctx->sdl.renderer);
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
        if(UIButtonIsPressed(evt, &contorlsButton)) {
            int code = UIPollScreen(ControlsScreen, ctx, evt);
            if(code == -1) return -1;
        }
    }

    return 69;
}

int ControlsScreen(void* context, SDL_Event* evt)
{
    castor_Context* ctx = context;
    SCREEN_HEADER(ctx);
    SDL_Renderer* renderer = ctx->sdl.renderer;
    UIFont* font = ctx->ui.font;

    const char* title = "Controls";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);

    const char* controls[] = {
        "[WASD] - Move",
        "[Shift] - Run",
        "[E] - Interact",
        "[ESC] - Pause",
        "[X] - Activate Glasses",
        "[Arrow Keys / Mouse] - Camera",
    };

    int result = 69;
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            result = -1;
            break;
        }

        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            return 0;
    }

    const int num_controls = sizeof(controls) / sizeof(controls[0]);
    int screen_width, screen_height;
    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);

    // Button style
    const int button_width = 300;
    const int button_height = 50;
    const int padding = 20;
    const int start_y = screen_height * 0.2 + 120;

    int title_w, title_h;
    TTF_SizeText(title_font.ttf, title, &title_w, &title_h);
    UITextRender(renderer, title, (screen_width - title_w) / 2, screen_height * 0.2, &title_font);

    for (int i = 0; i < num_controls; ++i) {
        int box_x = (screen_width - button_width) / 2;
        int box_y = start_y + i * (button_height + padding);

        // Shadow
        SDL_Rect shadow = { box_x + 3, box_y + 3, button_width, button_height };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_RenderFillRect(renderer, &shadow);

        // Background
        SDL_Rect box = { box_x, box_y, button_width, button_height };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &box);

        // Border
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &box);

        // Render text centered in the box
        int text_w, text_h;
        TTF_SizeText(font->ttf, controls[i], &text_w, &text_h);
        int text_x = box.x + (box.w - text_w) / 2;
        int text_y = box.y + (box.h - text_h) / 2;

        UITextRender(renderer, controls[i], text_x, text_y, font);
    }

    SDL_RenderPresent(renderer);
    UIFontClose(&title_font);
    return result;
}

int SettingsScreen(void* context, SDL_Event* evt)
{
    castor_Context* ctx = context;
    SCREEN_HEADER(ctx);
    const char* title = "Settings";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    
    size_t centerX = (ctx->sdl.screen_width - buttonsWidth) / 2;
    size_t padding = 20;
    size_t btn_index = 0;
    UIButton fullscreenButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(ctx->sdl.screen_height, buttonsHeight, padding, btn_index++),
        .label = (ctx->settings.fullscreen) ? "Window" : "Fullscreen",
    };
    // NOTE: Fullscreen button disabled because of known SDL bug
    fullscreenButton.disabled = true;
    fullscreenButton.color = fullscreenButton.disabled_color;
    
    static float sensitivity = 30;
    UISlider sensitivitySlider = {0};
    UISliderInit(&sensitivitySlider, 
            (ctx->sdl.screen_width - buttonsWidth) / 2,
            (fullscreenButton.y + fullscreenButton.h + padding) + padding,
            buttonsWidth, 15, 0, 100, sensitivity, 
            UI_COLOR_GRAY, UI_COLOR_RED);
    sensitivitySlider.dragging = false; // Explicitly initialize dragging state

    static float render_distance = 20;
    UISlider renderDistanceSlider = {0};
    UISliderInit(&renderDistanceSlider, 
            (ctx->sdl.screen_width - buttonsWidth) / 2,
            sensitivitySlider.y + fullscreenButton.h + padding,
            buttonsWidth, 15, 10, 50, render_distance, 
            UI_COLOR_GRAY, UI_COLOR_RED);
    sensitivitySlider.dragging = false; // Explicitly initialize dragging state

    
    btn_index++;
    btn_index++;
    UIButton backButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(ctx->sdl.screen_height, buttonsHeight, padding, btn_index++),
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
            castor_SetFullscreen(ctx, !ctx->settings.fullscreen);
            fullscreenButton.label = (ctx->settings.fullscreen) ? "Window" : "Fullscreen";
        }
    }
    
    sensitivity = sensitivitySlider.value;
    ctx->settings.mouse_sensitivity = sensitivity / 100;

    render_distance = renderDistanceSlider.value;
    ctx->settings.render_distance = render_distance;
    
    // Rendering starts here
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);
    
    UITextRender(ctx->sdl.renderer, title, (ctx->sdl.screen_width - text_w) / 2, SCREEN_TITLE_Y(ctx->sdl.screen_height), &title_font);
    UIButtonOnHover(evt, &fullscreenButton);
    UIButtonRender(ctx->sdl.renderer, &fullscreenButton);
    UIButtonOnHover(evt, &backButton);
    UIButtonRender(ctx->sdl.renderer, &backButton);

    UITextRender(ctx->sdl.renderer, "Sensitivity", sensitivitySlider.x, sensitivitySlider.y-sensitivitySlider.h - ctx->ui.font->size/2, ctx->ui.font);
    UISliderRender(ctx->sdl.renderer, &sensitivitySlider);
    UITextRender(ctx->sdl.renderer, "Render Distance", renderDistanceSlider.x, renderDistanceSlider.y-renderDistanceSlider.h - ctx->ui.font->size/2, ctx->ui.font);
    UISliderRender(ctx->sdl.renderer, &renderDistanceSlider);
    
    SDL_RenderPresent(ctx->sdl.renderer);
    UIFontClose(&title_font);
    
    return result;
}


static const char* hints[] = {
    "Tip: Use [WASD] keys to move around.",
    "Hint: Press [E] to interact with doors",
    "Remember: Keep an eye on your health bar at the bottom!",
    "Collect keys to unlock new areas!",
    "Find a sword to defend yourself",
    "Pause the game anytime with [ESC].",
    "Tip: Watch out for glitching textures they might reveal secrets!"
};
static int hint_index = -1;

int LoadingScreen(void* context, SDL_Event* evt)
{
    if(hint_index == -1)
        hint_index = rand() % (sizeof(hints) / sizeof(hints[0]));

    castor_Context* ctx = context;
    SCREEN_HEADER(ctx);
    const char* title = "Next level";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);
    int title_w, title_h;
    TTF_SizeText(title_font.ttf, title, &title_w, &title_h);
    const char* hint = hints[hint_index];
    int hint_w, hint_h;
    TTF_SizeText(ctx->ui.font->ttf, hint, &hint_w, &hint_h);


    size_t centerX = (ctx->sdl.screen_width - buttonsWidth) / 2;
    size_t padding = 20;
    UIButton continueButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(ctx->sdl.screen_height, buttonsHeight, padding, 2),
        .label = "Continue"
    };

    int result = 69;
    
    // Handle all events
    SDL_Event event;
    if(!evt) evt = &event;
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            result = -1;
            break;
        }

        if ((evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_RETURN) ||
                UIButtonIsPressed(evt, &continueButton)) {
            hint_index = -1;
            return 0;
        }
    }
    
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);
    
    UITextRender(ctx->sdl.renderer, title, (ctx->sdl.screen_width - title_w) / 2, SCREEN_TITLE_Y(ctx->sdl.screen_height), &title_font);
    UITextRender(ctx->sdl.renderer, hint, (ctx->sdl.screen_width - hint_w) / 2, NEXT_BUTTON_Y(ctx->sdl.screen_height, buttonsHeight, padding, 0), ctx->ui.font);
    UIButtonOnHover(evt, &continueButton);
    UIButtonRender(ctx->sdl.renderer, &continueButton);

    SDL_RenderPresent(ctx->sdl.renderer);
    UIFontClose(&title_font);

    return result;
}

int FailScreen(void* context, SDL_Event* evt)
{
    castor_Context* ctx = context;
    SCREEN_HEADER(ctx);
    const char* title = "You died";
    UIFont title_font = {0};
    UIFontOpen(&title_font, UI_GLOBAL_FONT, 60, UI_COLOR_WHITE);
    int text_w, text_h;
    TTF_SizeText(title_font.ttf, title, &text_w, &text_h);
    
    size_t centerX = (ctx->sdl.screen_width - buttonsWidth) / 2;
    size_t padding = 20;
    size_t btn_index = 0;
    UIButton continueButton = {
        BUTTON_DEFAULTS(ctx),
        .x = centerX,
        .y = NEXT_BUTTON_Y(ctx->sdl.screen_height, buttonsHeight, padding, btn_index++),
        .label = "Continue"
    };

    int result = 69;
    
    // Handle all events
    SDL_Event event;
    if(!evt) evt = &event;
    while (SDL_PollEvent(evt)) {
        if (evt->type == SDL_QUIT) {
            result = -1;
            break;
        }

        if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_RETURN) return 0;
        if (UIButtonIsPressed(evt, &continueButton)) {
            return 0;
        }
    }
    
    SDL_SetRenderDrawColor(ctx->sdl.renderer, 30, 30, 30, 255);
    SDL_RenderClear(ctx->sdl.renderer);
    
    UITextRender(ctx->sdl.renderer, title, (ctx->sdl.screen_width - text_w) / 2, SCREEN_TITLE_Y(ctx->sdl.screen_height), &title_font);
    UIButtonOnHover(evt, &continueButton);
    UIButtonRender(ctx->sdl.renderer, &continueButton);

    SDL_RenderPresent(ctx->sdl.renderer);
    UIFontClose(&title_font);

    return result;
}

