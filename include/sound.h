#ifndef SOUND_H
#define SOUND_H
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_thread.h>

typedef struct {
    SDL_Thread* thread;
    const char* file;
    int volume;
    Uint32 duration_ms;
} SoundThread;

#endif // SOUND_H
