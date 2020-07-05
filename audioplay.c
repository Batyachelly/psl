#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "include/audioplay.h"

void audioplay_play(struct audioplay *);
void audioplay_free(struct audioplay *);

void *audioplay_thread(void *arg)
{
    struct audioplay *audioplay = arg;

    SDL_Delay(250);
    int result = 0;
    int flags = MIX_INIT_MP3;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        log_c("Failed to init SDL", __func__, __LINE__, 2);
        exit(1);
    }

    if (flags != (result = Mix_Init(flags)))
    {
        log_c("Could not initialize mixer", __func__, __LINE__, 2);
        // log_c(Mix_GetError(), __func__, __LINE__, 0);

        // printf("Could not initialize mixer (result: %d).\n", result);
        exit(1);
    }

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640))
    {
        log_c("Could not init audio", __func__, __LINE__, 2);
    }

    char out[50];

    Mix_Music *music = Mix_LoadMUS(audioplay->filename);

    if (music)
    {
        log_c("Could not open music", __func__, __LINE__, 2);
        for (size_t i = 0; i < 5; i++)
        {
            SDL_Delay(1000);
            sprintf(out, "Try(%ld) to open music", i);
            log_c(out, __func__, __LINE__, 0);
            music = Mix_LoadMUS(audioplay->filename);
            if (music)
            {
                break;
            }
        }
    }


    if (Mix_PlayMusic(music, 1))
    {
        log_c("Could not start play", __func__, __LINE__, 2);
        for (size_t i = 0; i < 5; i++)
        {
            SDL_Delay(1000);
            sprintf(out, "Try(%ld) to start play", i);
            log_c(out, __func__, __LINE__, 0);
            if (!Mix_PlayMusic(music, 1))
            {
                break;
            }
        }
        // log_c(Mix_GetError(), __func__, __LINE__, 0);
    }

    while (!SDL_QuitRequested())
    {
        SDL_Delay(250);
    }

    Mix_FreeMusic(music);
    SDL_Quit();
}

struct audioplay *audioplay_init(char *filename)
{
    struct audioplay *audioplay = malloc(sizeof(struct audioplay));
    audioplay->play = audioplay_play;
    audioplay->free = audioplay_free;
    strcpy(audioplay->filename, filename);
    log_c("SLDMixer was inited", __func__, __LINE__, 1);
    return audioplay;
}

void audioplay_free(struct audioplay *audioplay)
{
    pthread_cancel(audioplay->audioplay_id);
    free(audioplay);
    log_c("SLDMixer was cleaned", __func__, __LINE__, 1);
}

void audioplay_play(struct audioplay *audioplay)
{
    log_c("SLDMixer start thread", __func__, __LINE__, 1);
    pthread_create(&audioplay->audioplay_id, NULL, audioplay_thread, audioplay);
}

// void audioplay_stop(struct audioplay *audioplay)
// {
//     pthread_cancel(audioplay->audioplay_id);
// }
