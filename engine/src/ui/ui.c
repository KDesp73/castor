#include "ui.h"
#include "core.h"

void UIOpen(UI* ui, UIFont* font)
{
    ui->font = font;
}

void UIRender(UI* ui, void* context)
{
    castor_Context* ctx = context;

    // TOASTS
    for(size_t i = 0; i < ui->toast_count; i++){
        UIToast toast = ui->toasts[i];
        if (toast.active) {
            Uint32 elapsed_time = SDL_GetTicks() - toast.start_time;
            if (elapsed_time < toast.duration) {
                UIToastRender(ctx->sdl.renderer, ui->font, &toast, ctx->sdl.screen_width, ctx->sdl.screen_height);
            } else {
                toast.active = false;
            }
        }
    }
}

void UIClose(UI* ui)
{
    UIFontClose(ui->font);
}

void UIAppendToast(UI* ui, UIToast toast)
{
    if(ui->toast_count >= MAX_TOASTS) ui->toast_count = 0;

    ui->toasts[ui->toast_count++] = toast;
}
