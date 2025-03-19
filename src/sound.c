#include "sound.h"
#include "context.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACK_SIZE 1024  // You can adjust this size depending on your needs

typedef struct {
    const char* file;
    int volume;
    Uint32 duration_ms;
} SoundThreadData;

static int PlaySoundThread(void* data)
{
    SoundThreadData* soundData = (SoundThreadData*)data;

    Mix_Chunk* sound = Mix_LoadWAV(soundData->file);
    if (!sound) {
        printf("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
        free(soundData);
        return 1;
    }

    Mix_VolumeChunk(sound, soundData->volume);

    int channel = Mix_PlayChannel(-1, sound, 0);
    if (channel == -1) {
        printf("Failed to play sound! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_FreeChunk(sound);
        free(soundData);
        return 1;
    }

    SDL_Delay(soundData->duration_ms);

    Mix_HaltChannel(channel);
    Mix_FreeChunk(sound);

    free(soundData);

    return 0;
}

int PlaySound(Context* ctx, const char* file, int volume, Uint32 duration_ms)
{
    if (!file || file[0] == '\0') {
        printf("Invalid file path provided!\n");
        return -1;
    }

    if (ctx->sound_thread_count >= MAX_SOUND_THREADS) {
        CleanupThreads(ctx);
        return 1;
    }

    SoundThreadData* data = (SoundThreadData*)malloc(sizeof(SoundThreadData));
    if (!data) {
        printf("Failed to allocate memory for sound data!\n");
        return -1;
    }

    // Copy the data into the struct
    data->file = file;
    data->volume = volume;
    data->duration_ms = duration_ms;

    // Create the thread and pass the struct pointer as the argument
    SDL_Thread* thread = SDL_CreateThread(PlaySoundThread, "SoundThread", data);
    if (!thread) {
        printf("Failed to create thread! SDL Error: %s\n", SDL_GetError());
        free(data);  // Free the allocated memory if thread creation fails
        return -1;
    }

    ctx->sound_threads[ctx->sound_thread_count].thread = thread;
    ctx->sound_threads[ctx->sound_thread_count].file = data->file;
    ctx->sound_threads[ctx->sound_thread_count].duration_ms = data->duration_ms;
    ctx->sound_threads[ctx->sound_thread_count].volume = data->volume;
    ctx->sound_thread_count++;

    return 0;
}

void CleanupThreads(Context* ctx)
{
    int new_count = 0;

    for (int i = 0; i < ctx->sound_thread_count; ++i) {
        if (ctx->sound_threads[i].thread != NULL) {
            int status;
            SDL_WaitThread(ctx->sound_threads[i].thread, &status);

            ctx->sound_threads[i].thread = NULL;

            if (status == 0) {
                ctx->sound_threads[new_count++] = ctx->sound_threads[i];
            }
        }
    }

    ctx->sound_thread_count = new_count;
}
