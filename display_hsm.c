#include "display_hsm.h"
#include <string.h>
#include "utils_hsm.h"

void DISPLAY_INIT_HSM(Display_hsm *const display){
    display->state = DISPLAY_OFF_STATE;
    display->event = NO_EVENT;
    strcpy(display->message, "00:00:00");
    display->timer = 0;
    display->refresh_ticks = DISPLAY_REFRESH_TICKS_PERIOD;
    DISPLAY_DEVICE_INIT();
}


void DISPLAY_RECEIVE_EVENT(Display_hsm * display, HsmEvent event){
    display->event = event;
}

void RUN_DISPLAY_HSM(Display_hsm * display){
    Hsm_state next_state;
    while((next_state = display->state(display)) != 0){
        display->state = (State_function) next_state;
    }
}

State_function DISPLAY_ON_STATE_HANDLER(Display_hsm* display){
    if(display->event == DISPLAY_OFF_EVENT){
        return DISPLAY_OFF_STATE;
    }
    DISPLAY_UPDATE(display);
    return 0;
}
void DISPLAY_UPDATE(Display_hsm* display){
    display->refresh_ticks--;
    if(display->refresh_ticks < 0){
        DISPLAY_UPDATE_MESSAGE(display);
        DISPLAY_DEVICE_WRITE_STRING(display->message);
        display->refresh_ticks = DISPLAY_REFRESH_TICKS_PERIOD;
    }
    display->state == DISPLAY_ON_STATE;
}

State_function DISPLAY_OFF_STATE_HANDLER(Display_hsm * display){
    if(display->event == DISPLAY_ON_EVENT){
        return DISPLAY_ON_STATE;
    }
    DISPLAY_DEVICE_CLEAR();
    return 0;
}



void DISPLAY_UPDATE_MESSAGE(Display_hsm *const display){
    DISPLAY_CONVERT_TIME_TO_STRING(display->message, display->timer);
}

void DISPLAY_CONVERT_TIME_TO_STRING(char *const s, const TimeCounter *const t){
    const int NUM_SIZE = 2;
    int i=0;
    if(TIME_COUNTER_IS_NEGATIVE(t)){
        s[i++] = '-';
    }
    unsigned_int_to_string(s+i, t->hours, NUM_SIZE);
    i+=NUM_SIZE;
    s[i++] = ':';
    unsigned_int_to_string(s+i, t->minutes, NUM_SIZE);
    i+=NUM_SIZE;
    s[i++] = ':';
    unsigned_int_to_string(s+i, t->seconds, NUM_SIZE);
}