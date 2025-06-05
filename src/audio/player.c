#include <audio/player.h>

static SDL_AudioStream *stream = NULL;
static Uint8 *wav_data = NULL;
static Uint32 wav_data_len = 0;

void VibeCast_PlayAudio(const char *filename)
{
    if (!filename)
        return;

    SDL_AudioSpec spec;
    char *wav_path = NULL;

    SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), filename); /* allocate a string of the full file path */
    if (!SDL_LoadWAV(wav_path, &spec, &wav_data, &wav_data_len))
    {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        SDL_free(wav_path); /* done with this string. */
        return;
    }

    SDL_free(wav_path); /* done with this string. */

    /* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (!stream)
    {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return;
    }

    VibeCast_ResumeAudio();
}

void VibeCast_PauseAudio()
{
    SDL_PauseAudioStreamDevice(stream);
}

void VibeCast_ResumeAudio()
{
    /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
    SDL_ResumeAudioStreamDevice(stream);
}
