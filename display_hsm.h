#ifndef DISPLAY_HSM_H
#define DISPLAY_HSM_H
#include "hsm_header.h"
#include "time_counter_hsm.h"
#include "device_interface.h"

#define DISPLAY_MESSAGE_LENGTH (9+1)
#define DISPLAY_REFRESH_TICKS_PERIOD (500)
typedef struct Display_hsm{
    State_function state;
    HsmEvent event;
    char message[DISPLAY_MESSAGE_LENGTH];
    TimeCounter *timer;
    int show_minus;
    int refresh_ticks;
}Display_hsm;

void DISPLAY_INIT_HSM(Display_hsm *const display);
void DISPLAY_RECEIVE_EVENT(Display_hsm *const display, HsmEvent event);
void DISPLAY_UPDATE_MESSAGE(Display_hsm *const display);
void DISPLAY_ADD_MINUS_TO_MESSAGE(Display_hsm *const display);
void DISPLAY_UPDATE_DEVICE(Display_hsm *const display);
void DISPLAY_UPDATE(Display_hsm *const display);
void RUN_DISPLAY_HSM(Display_hsm * display);

void DISPLAY_CONVERT_TIME_TO_STRING(char *const s, const TimeCounter *const t);
void int_to_string(char*const s, const int i, const int NUM_SIZE);
State_function DISPLAY_ON_STATE_HANDLER(Display_hsm* display);
State_function DISPLAY_OFF_STATE_HANDLER(Display_hsm * display);

#define DISPLAY_ON_STATE (State_function)DISPLAY_ON_STATE_HANDLER
#define DISPLAY_OFF_STATE (State_function)DISPLAY_OFF_STATE_HANDLER

#endif