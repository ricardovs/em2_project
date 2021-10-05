#ifndef MUSIC_H
#define MUSIC_H

#include "device_interface.h"
#include "hsm_header.h"
#include "frg_hsm.h"

//Defining Periods
#define MUSIC_HSM_TOGGLE_PERIOD 500
#define MUSIC_HSM_STOP_PERIOD 10000
#define MUSIC_HSM_BEEP_PERIOD 1000
#define MUSIC_HSM_BEEP_TOGGLE_PERIOD 200

//Defining Sounds
#define MUSIC_HSM_BEEP_SOUND DEVICE_SOUND_D
#define MUSIC_HSM_FINAL_SOUND1 DEVICE_SOUND_C
#define MUSIC_HSM_FINAL_SOUND2 DEVICE_SOUND_E

typedef struct Music_hsm
{
    State_function state;
    unsigned count_toggle;
    unsigned count_transition;
    Device_sound tone;
}Music_hsm;

void MUSIC_INIT_HSM(Music_hsm *const hsm);
void RUN_MUSIC(Music_hsm *const hsm);
void MUSIC_TRANS(Music_hsm *const hsm, const State_function next_state);

State_function MUSIC_IDLE_STATE_HANDLE(Music_hsm *const hsm);
State_function MUSIC_PLAY_FINAL_STATE_HANDLE(Music_hsm *const hsm);
State_function MUSIC_BEEP_STATE_HANDLE(Music_hsm *const hsm);

#define MUSIC_IDLE_STATE (State_function)MUSIC_IDLE_STATE_HANDLE
#define MUSIC_PLAY_FINAL_STATE (State_function)MUSIC_PLAY_FINAL_STATE_HANDLE
#define MUSIC_PLAY_FINAL_STATE (State_function)MUSIC_PLAY_FINAL_STATE_HANDLE
#endif // MUSIC_H
