#include "ui.h"

void UISliderInit(UISlider *slider, int x, int y, int w, int h, float min, float max, float initial, SDL_Color barColor, SDL_Color knobColor) 
{
    if (!slider) return;
    
    slider->x = x;
    slider->y = y;
    slider->w = w;
    slider->h = h;
    slider->min = min;
    slider->max = max;
    slider->value = (initial < min) ? min : (initial > max) ? max : initial;
    slider->barColor = barColor;
    slider->knobColor = knobColor;
    slider->dragging = false;
}


#define SLIDER_X_TOLERANCE 5
#define SLIDER_Y_TOLERANCE 5
void UISliderRender(SDL_Renderer *renderer, UISlider *slider)
{
    // Draw the slider track
    SDL_Rect track = { slider->x, slider->y + slider->h / 2 - 2, slider->w, 4 };
    SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(slider->barColor));
    SDL_RenderFillRect(renderer, &track);
    
    // Calculate knob position - center the knob on the value position
    int knobWidth = 20;
    int knobX = slider->x + (int)(((slider->value - slider->min) / (slider->max - slider->min)) * slider->w);
    
    // Position the knob so it's centered on the calculated position
    SDL_Rect knob = { knobX - (knobWidth / 2), slider->y, knobWidth, slider->h };
    
    SDL_SetRenderDrawColor(renderer, UI_COLOR_PARAMS(slider->knobColor));
    SDL_RenderFillRect(renderer, &knob);
    
    // Debug visualization - uncomment to see the hitbox
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    // SDL_Rect hitbox = { knobX - (knobWidth / 2) - SLIDER_X_TOLERANCE, slider->y - SLIDER_Y_TOLERANCE, 
    //                     knobWidth + 2*SLIDER_X_TOLERANCE, slider->h + 2*SLIDER_Y_TOLERANCE };
    // SDL_RenderDrawRect(renderer, &hitbox);
}

void UISliderHandleEvent(UISlider *slider, SDL_Event *event)
{
    if (!slider || !event) return;
    
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Handle mouse button down - start dragging
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        // Check if click is within the slider track area for better usability
        if (mouseX >= slider->x && mouseX <= slider->x + slider->w &&
            mouseY >= slider->y - SLIDER_Y_TOLERANCE && mouseY <= slider->y + slider->h + SLIDER_Y_TOLERANCE) {
            
            slider->dragging = true;
            
            // Immediately update slider value based on click position
            int newPos = mouseX - slider->x;
            newPos = (newPos < 0) ? 0 : (newPos > slider->w ? slider->w : newPos);
            slider->value = slider->min + ((double)newPos / slider->w) * (slider->max - slider->min);
        }
    }
    // Handle mouse button up - stop dragging
    else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        slider->dragging = false;
    }
    // Handle mouse motion - update value if dragging
    else if (event->type == SDL_MOUSEMOTION) {
        // Only update if we're already dragging
        if (slider->dragging) {
            int newPos = mouseX - slider->x;
            newPos = (newPos < 0) ? 0 : (newPos > slider->w ? slider->w : newPos);
            slider->value = slider->min + ((double)newPos / slider->w) * (slider->max - slider->min);
        }
    }
}
