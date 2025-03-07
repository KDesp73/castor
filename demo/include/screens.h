#ifndef SCREENS_H
#define SCREENS_H

#include "ui.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

int StartScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui);
int PauseScreen(SDL_Renderer* renderer, SDL_Event* evt, UI* ui);

#endif // SCREENS_H
