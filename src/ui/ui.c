#include "ui.h"

void UIOpen(UI* ui, Context* ctx, UIFont* font)
{
    ui->ctx = ctx;
    ui->font = font;
}

void UIClose(UI* ui)
{
    UIFontClose(ui->font);
}

