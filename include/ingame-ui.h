#ifndef IN_GAME_UI_H
#define IN_GAME_UI_H
#include "context.h"
#include <SDL2/SDL_render.h>


void AddDamageNumber(Context* ctx, float enemy_x, float enemy_y, int damage);
void RenderDamageNumbers(Context* ctx);
void UpdateDamageNumbers(Context* ctx);
void RenderCrosshair(SDL_Renderer* renderer, int screen_width, int screen_height);

#endif // IN_GAME_UI_H
