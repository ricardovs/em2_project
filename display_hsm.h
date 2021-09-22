#ifndef DISPLAY_HSM_H
#define DISPLAY_HSM_H
#include "hsm_header.h"
#include "time_counter_hsm.h"
#define DISPLAY_MESSAGE_LENGTH 10

typedef struct Display_hsm{
    State_function state;
    HsmEvent event;
    char message[DISPLAY_MESSAGE_LENGTH];
    TimeCounter *timer;
}Display_hsm;

void DISPLAY_INIT_HSM(Display_hsm *const display);
void DISPLAY_RECEIVE_EVENT(Display_hsm *const display, HsmEvent event);
void RUN_DISPLAY_HSM(Display_hsm * display);

State_function DISPLAY_ON_STATE_HANDLER(Display_hsm* display);
State_function DISPLAY_OFF_STATE_HANDLER(Display_hsm * display);

#define DISPLAY_ON_STATE (State_function)DISPLAY_ON_STATE_HANDLER
#define DISPLAY_OFF_STATE (State_function)DISPLAY_OFF_STATE_HANDLER

#endif