#ifndef CASTOR_ASSET_H
#define CASTOR_ASSET_H

#include <stdbool.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_thread.h>

// ANIMATION
typedef struct {
    SDL_Texture *texture;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    size_t currentFrame;
    Uint32 lastUpdateTime;
    Uint32 frameDelay;
} castor_Animation;

castor_Animation castor_LoadAnimation(SDL_Renderer *renderer, const char *filePath, int frameWidth, int frameHeight, Uint32 rate);
void castor_RenderAnimation(SDL_Renderer *renderer, castor_Animation *anim, float scale, int x, int y, int frameIndex);
void castor_UpdateAnimation(castor_Animation *anim, Uint32 currentTime);
void castor_FreeAnimation(castor_Animation *anim);

// IMAGE
typedef struct {
    SDL_Texture* texture;
    int w, h;
} castor_Image;

castor_Image castor_LoadImage(SDL_Renderer* renderer, const char* path);
void castor_RenderImage(SDL_Renderer* renderer, castor_Image* image, float scale, int x, int y);
void castor_RenderRotatedImage(SDL_Renderer* renderer, castor_Image* image, float scale, int x, int y, double angle);
void castor_FreeImage(castor_Image* image);

// SOUND
typedef struct {
    SDL_Thread* thread;
    const char* file;
    int volume;
    Uint32 duration_ms;
} castor_SoundThread;


// TEXTURES
#define TEXTURES_LIST_FILE "assets/textures.list"
#define SPRITES_LIST_FILE "assets/sprites.list"

SDL_Texture* castor_TextureLoad(SDL_Renderer *renderer, const char* file_path);
bool castor_TexturesLoad(SDL_Renderer* renderer, SDL_Texture** textures, const char* path);

#endif // CASTOR_ASSET_H
