#include <audio/player.h>
#include <SDL3/SDL.h>

/* Variables utilizadas para el audio */
static SDL_AudioStream *stream = NULL; // Flujo de salida para audio (como stdout)
static SDL_AudioSpec spec;             // Características del sonido para el dispositivo de audio
static Uint8 *wav_data = NULL;         // Para guardar la data de un WAV
static Uint32 wav_data_len = 0;        // Longitud de la data de un WAV

void VibeCast_InitAudio()
{
    if (!stream)
    {
        stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);
        VibeCast_ResumeAudio();
    }
}

void VibeCast_PlayAudio(const char *filename)
{
    if (!stream || !filename)
        return;

    char *wav_path = NULL;
    /* La ruta completa del ar chivo */
    SDL_asprintf(&wav_path, "%s%s", SDL_GetBasePath(), filename); /* allocate a string of the full file path */

    /* Cargar el WAV */
    if (!SDL_LoadWAV(wav_path, &spec, &wav_data, &wav_data_len))
    {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        SDL_free(wav_path); /* done with this string. */
        return;
    }

    SDL_free(wav_path); /* done with this string. */

    /* Cargar el formato del audio */
    if (!SDL_SetAudioStreamFormat(stream, NULL, &spec))
    {
        SDL_Log("Couldn't set audio stream format: %s", SDL_GetError());
        return;
    }

    VibeCast_ResumeAudio();
}

void VibeCast_PauseAudio()
{
    if (!stream)
        return;
    SDL_PauseAudioStreamDevice(stream);
}

void VibeCast_ResumeAudio()
{
    if (!stream)
        return;
    /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
    SDL_ResumeAudioStreamDevice(stream);
}
