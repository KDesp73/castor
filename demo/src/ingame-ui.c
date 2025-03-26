#include "context.h"
#include "ingame-ui.h"

void AddDamageNumber(Context* ctx, float enemy_x, float enemy_y, int damage)
{
    if (ctx->ui.damage_number_count >= MAX_DAMAGE_NUMBERS) return;

    DamageNumber dn;

    dn.x = enemy_x + (rand() % 20 - 10); // random offset X [-10, 10]
    dn.y = enemy_y - 30 + (rand() % 10 - 5); // random offset Y

    // Floating velocity (rising effect)
    dn.vx = (rand() % 10 - 5) / 10.0f; // small random horizontal drift
    dn.vy = -0.5f; // constant upward speed

    dn.damage = damage;
    dn.angle = (rand() % 360); // random rotation angle
    dn.scale = 2.0f + (rand() % 50) / 100.0f; // scale between 2.0 and 2.5
    dn.spawn_time = SDL_GetTicks();
    dn.lifetime = 2000; // 1 second duration

    ctx->ui.damage_numbers[ctx->ui.damage_number_count++] = dn;
}

void UpdateDamageNumbers(Context* ctx)
{
    Uint32 now = SDL_GetTicks();
    for (int i = 0; i < ctx->ui.damage_number_count; i++) {
        DamageNumber* dn = &ctx->ui.damage_numbers[i];

        dn->x += dn->vx;
        dn->y += dn->vy;

        if (now - dn->spawn_time > dn->lifetime) {
            ctx->ui.damage_numbers[i] = ctx->ui.damage_numbers[ctx->ui.damage_number_count - 1];
            ctx->ui.damage_number_count--;
            i--;
        }
    }
}

static void RenderText(float x, float y, float damage, float scale, float angle, float alpha, SDL_Renderer* renderer, TTF_Font* font)
{
    char text[16];
    snprintf(text, sizeof(text), "%.0f", damage);

    SDL_Color color = {255, 0, 0, (Uint8)(alpha * 255)}; // Red color, fading alpha

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) {
        SDL_Log("Failed to render text surface: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect;
    dstRect.w = (int)(surface->w * scale);
    dstRect.h = (int)(surface->h * scale);
    dstRect.x = (int)(x - dstRect.w / 2); // Centered
    dstRect.y = (int)(y - dstRect.h / 2);

    SDL_SetTextureAlphaMod(texture, (Uint8)(alpha * 255));

    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void RenderDamageNumbers(Context* ctx)
{
    Uint32 now = SDL_GetTicks();

    for (int i = 0; i < ctx->ui.damage_number_count; i++) {
        DamageNumber* dn = &ctx->ui.damage_numbers[i];

        // Fade out effect
        float alpha = 1.0f - ((float)(now - dn->spawn_time) / dn->lifetime);
        if (alpha < 0) alpha = 0;

        RenderText(
            dn->x,
            dn->y,
            dn->damage,
            dn->scale,
            dn->angle,
            alpha,
            ctx->sdl.renderer,
            ctx->ui.font->ttf
        );
    }
}

void RenderCrosshair(SDL_Renderer* renderer, int screen_width, int screen_height)
{
    int centerX = screen_width / 2;
    int centerY = screen_height / 2;
    int size = 15;
    int gap = 6;
    int thickness = 4;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, centerX - size, centerY + i, centerX - gap, centerY + i);
        SDL_RenderDrawLine(renderer, centerX + gap, centerY + i, centerX + size, centerY + i);
    }

    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        SDL_RenderDrawLine(renderer, centerX + i, centerY - size, centerX + i, centerY - gap);
        SDL_RenderDrawLine(renderer, centerX + i, centerY + gap, centerX + i, centerY + size);
    }
}

void RenderHealthBar(SDL_Renderer* renderer, int x, int y, int width, int height, int currentHealth, int maxHealth)
{
    // Clamp health values
    if (currentHealth < 0) currentHealth = 0;
    if (currentHealth > maxHealth) currentHealth = maxHealth;

    // Background (black)
    SDL_Rect bgRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    // Health bar (red)
    float healthPercent = (float)currentHealth / (float)maxHealth;
    int healthWidth = (int)(width * healthPercent);

    SDL_Rect healthRect = { x, y, healthWidth, height };
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthRect);

    // Optional: Border (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}

void RenderGlassesCooldown(SDL_Renderer* renderer, int x, int y, int width, int height, int currentCooldown, int maxCooldown)
{
    // Clamp health values
    if (currentCooldown < 0) currentCooldown = 0;
    if (currentCooldown > maxCooldown) currentCooldown = maxCooldown;

    // Background (black)
    SDL_Rect bgRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    // Cooldown bar (red)
    float cooldownPercent = (float)currentCooldown / (float)maxCooldown;
    int cooldownWidth = (int)(width * cooldownPercent);

    SDL_Rect cooldownRect = { x, y, cooldownWidth, height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
    SDL_RenderFillRect(renderer, &cooldownRect);

    // Optional: Border (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}
