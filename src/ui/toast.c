#include "ui.h"
#include <SDL2/SDL_timer.h>

void UIToastInit(UIToast *toast, const char *message, int duration_ms)
{
    strncpy(toast->message, message, sizeof(toast->message) - 1);
    toast->duration = duration_ms;
    toast->start_time = SDL_GetTicks();
    toast->active = true;
}

#define MAX_TOAST_LINES 10
void UIToastRender(SDL_Renderer *renderer, UIFont *font, UIToast *toast, int screen_width, int screen_height)
{
    if (!toast->active) return;
    if(toast->start_time == 0)
        toast->start_time = SDL_GetTicks();

    Uint32 elapsed = SDL_GetTicks() - toast->start_time;
    if (elapsed > toast->duration) {
        toast->active = false;  // Hide toast after time expires
        return;
    }

    int max_width = screen_width / 3; // Toast max width is half the screen
    int line_height = TTF_FontHeight(font->ttf);

    // Word wrapping logic
    char *lines[MAX_TOAST_LINES] = {0};
    int num_lines = 0;
    char temp_msg[256];
    strncpy(temp_msg, toast->message, sizeof(temp_msg));
    temp_msg[sizeof(temp_msg) - 1] = '\0';

    char *token = strtok(temp_msg, " ");
    char current_line[256] = "";
    
    while (token && num_lines < MAX_TOAST_LINES) {
        char test_line[256];
        strncpy(test_line, current_line, sizeof(test_line) - 1);
        test_line[sizeof(test_line) - 1] = '\0'; // Ensure null termination
        strncat(test_line, " ", sizeof(test_line) - strlen(test_line) - 1);
        strncat(test_line, token, sizeof(test_line) - strlen(test_line) - 1);

        int text_w;
        TTF_SizeText(font->ttf, test_line, &text_w, NULL);

        if (text_w > max_width && strlen(current_line) > 0) {
            lines[num_lines++] = strdup(current_line);
            current_line[0] = '\0'; // Reset current line
        }

        strncat(current_line, token, sizeof(current_line) - strlen(current_line) - 1);
        strncat(current_line, " ", sizeof(current_line) - strlen(current_line) - 1);

        token = strtok(NULL, " ");
    }
    if (strlen(current_line) > 0 && num_lines < MAX_TOAST_LINES) {
        lines[num_lines++] = strdup(current_line);
    }

    // Compute toast box dimensions
    int padding = 10;
    int box_w = max_width + 2 * padding;
    int box_h = (line_height * num_lines) + 2 * padding;
    int box_x = screen_width - box_w - 20;  // 20px from right edge
    int box_y = screen_height - box_h - 20; // 20px from bottom edge

    // Fade-in/out effect (500ms fade time)
    Uint8 alpha = 255;
    if (elapsed < 500) alpha = (Uint8)((elapsed / 500.0f) * 255);
    if (elapsed > toast->duration - 500) alpha = (Uint8)(((toast->duration - elapsed) / 500.0f) * 255);

    int text_x = box_x + padding;
    int text_y = box_y + padding;
    for (int i = 0; i < num_lines; i++) {
        SDL_Surface *textSurface = TTF_RenderText_Solid(font->ttf, lines[i], font->color);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {text_x, text_y, textSurface->w, textSurface->h};

            SDL_SetTextureAlphaMod(textTexture, alpha);
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            text_y += line_height;  // Move down for next line
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
        free(lines[i]); // Free allocated memory for text lines
    }
}
