#ifndef IN_GAME_UI_H
#define IN_GAME_UI_H
#include "core.h"
#include <SDL2/SDL_render.h>

#define UI_GLOBAL_FONT "assets/fonts/Karla-Bold.ttf"

void AddDamageNumber(castor_Context* ctx, float enemy_x, float enemy_y, int damage);
void RenderDamageNumbers(castor_Context* ctx);
void UpdateDamageNumbers(castor_Context* ctx);
void RenderCrosshair(SDL_Renderer* renderer, int screen_width, int screen_height);
void RenderHealthBar(SDL_Renderer* renderer, int x, int y, int width, int height, int currentHealth, int maxHealth);
void RenderGlassesCooldown(SDL_Renderer* renderer, int x, int y, int width, int height, int currentCooldown, int maxCooldown);

#define NUM_PARTICLES 200
#define RECT_WIDTH 10   // Width of the confetti rectangles
#define RECT_HEIGHT 5   // Height of the confetti rectangles

typedef struct {
    float x, y;        // Position
    float vx, vy;      // Velocity
    SDL_Color color;   // Color of the particle
    int lifetime;      // Remaining lifetime of the particle
    int width, height; // Width and height of the rectangle
} Particle;

extern Particle particles[NUM_PARTICLES];


void initParticles(castor_Context* ctx);
void updateParticles(castor_Context* ctx);
void renderParticles(SDL_Renderer *renderer);


extern bool GlitchActivated;

#endif // IN_GAME_UI_H
