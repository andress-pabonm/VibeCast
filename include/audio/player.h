#ifndef VIBECAST_PLAYER_H
#define VIBECAST_PLAYER_H

#include <SDL3/SDL.h>
#include <stdbool.h>

bool VibeCast_InitAudio();

void VibeCast_PlayAudio(const char *wavPath);
void VibeCast_PauseAudio();
void VibeCast_ResumeAudio();

#endif