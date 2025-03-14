#ifndef SCREENS_H
#define SCREENS_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

int StartScreen(void* ctx, SDL_Event* evt);
int PauseScreen(void* ctx, SDL_Event* evt);
int SettingsScreen(void* ctx, SDL_Event* evt);

#endif // SCREENS_H
