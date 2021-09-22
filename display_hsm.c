#include "display_hsm.h"
#include <string.h>

void DISPLAY_INIT_HSM(Display_hsm *const display){
    display->state = DISPLAY_OFF_STATE;
    display->event = NO_EVENT;
    strcpy(display->message, "00:00:00");
    display->timer = 0;
}


void DISPLAY_RECEIVE_EVENT(Display_hsm * display, HsmEvent event){
    display->event = event;
}

void RUN_DISPLAY_HSM(Display_hsm * display){
    Hsm_state next_state;
    while((next_state = display->state(display)) != NULL){
        display->state = (State_function) next_state;
    }
}

State_function DISPLAY_ON_STATE_HANDLER(Display_hsm* display){
    if(display->event == DISPLAY_OFF_EVENT){
        return DISPLAY_OFF_STATE;
    }
    return 0;
}


State_function DISPLAY_OFF_STATE_HANDLER(Display_hsm * display){
    if(display->event == DISPLAY_ON_EVENT){
        return DISPLAY_ON_STATE;
    }
    return 0;
}
