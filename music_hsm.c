#include "music_hsm.h"

void MUSIC_INIT_HSM(Music_hsm *const hsm){
    hsm->state = MUSIC_IDLE_STATE;
    hsm->count_transition = 0;
    hsm->count_toggle = 0;
    hsm->tone = MUSIC_HSM_BEEP_SOUND;
    MUSIC_DEVICE_INIT();
}
void RUN_MUSIC(Music_hsm *const hsm){
    hsm->state(hsm);
}
void MUSIC_TRANS(Music_hsm *const  hsm,const State_function next_state){
    MUSIC_DEVICE_STOP();
    if(next_state == MUSIC_IDLE_STATE){
        hsm->state = next_state;
        return;
    }
    if(next_state == MUSIC_PLAY_FINAL_STATE){
        hsm->count_transition = MUSIC_HSM_STOP_PERIOD;
        hsm->count_toggle = MUSIC_HSM_TOGGLE_PERIOD;
        hsm->tone = MUSIC_HSM_FINAL_SOUND1;

    }else{
    //if next_state == MUSIC_PLAY_BEEP_STATE
        hsm->count_transition = MUSIC_HSM_BEEP_PERIOD;
        hsm->count_toggle = MUSIC_HSM_BEEP_TOGGLE_PERIOD;
        hsm->tone = MUSIC_HSM_BEEP_SOUND;
    }
    MUSIC_DEVICE_SET_SOUND(hsm->tone);
    MUSIC_DEVICE_PLAY();
    hsm->state = next_state;
}

State_function MUSIC_IDLE_STATE_HANDLE(Music_hsm *const  hsm){
    return 0;
}

State_function MUSIC_PLAY_FINAL_STATE_HANDLE(Music_hsm *const hsm){
    return 0;
}

State_function MUSIC_BEEP_STATE_HANDLE(Music_hsm *const hsm){
    hsm->count_transition--;
    if(hsm->count_transition <= 0){
        hsm->count_transition = MUSIC_HSM_BEEP_PERIOD;
        if(hsm->count_toggle > 0){
            MUSIC_DEVICE_STOP();
        }
        hsm->count_toggle = MUSIC_HSM_BEEP_TOGGLE_PERIOD;
        return 0;
    }
    hsm->count_toggle--;
    if(hsm->count_toggle > 0){
        if(!MUSIC_DEVICE_IS_PLAYING()){
            MUSIC_DEVICE_SET_SOUND(hsm->tone);
            MUSIC_DEVICE_PLAY();
        }
        return 0;
    }
    hsm->count_toggle = 0;  //Avoid underflow
    if(MUSIC_DEVICE_IS_PLAYING()){
        MUSIC_DEVICE_STOP();
    }
}
