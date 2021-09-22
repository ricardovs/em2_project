#ifndef TOP_HSM_H
#define TOP_HSM_H
#include "hsm_header.h"
#include "hsm_event.h"
#include "dial_hsm.h"
#include "display_hsm.h"
#include "time_counter_hsm.h"

typedef struct Top_hsm{
    State_function state;
    TimeCounter counter;
    TimeCounter updater;
    HsmEvent event;
    Dial_hsm dial;
    Display_hsm display;
    Dispatch_function event_dispatcher;
}Top_hsm;

void INIT_TOP_HSM(Top_hsm * top, void * outside_listener);
void INIT_ALL_TOP_HSMS(Top_hsm * top);
void RUN_TOP_HSM(Top_hsm * top);
void TOP_EVENT_DISPATCHER(HsmEvent * event);

State_function TOP_OFF_STATE_HANDLER(Top_hsm * top);
State_function TOP_SETTING_COUNTER_STATE_HANDLER(Top_hsm * top);
    State_function TOP_DISPLAYING_COUNTER_SET_STATE_HANDLER(Top_hsm * top);
State_function TOP_COUNTING_STATE_HANDLER(Top_hsm * top);
    State_function TOP_COUNTING_RESET_STATE_HANDLER(Top_hsm * top);
    State_function TOP_COUNTING_UPDATE_STATE_HANDLER(Top_hsm * top);
        State_function TOP_SETTING_UPDATE_STATE_HANDLER(Top_hsm * top);
            State_function TOP_DISPLAYING_UPDATER_SET_STATE_HANDLER(Top_hsm * top);
State_function TOP_ENDED_COUNTING_STATE_HANDLER(Top_hsm * top);

#define TOP_OFF_STATE (State_function)TOP_OFF_STATE_HANDLER
#define TOP_SETTING_COUNTER_STATE (State_function)TOP_SETTING_COUNTER_STATE_HANDLER
#define TOP_COUNTING_STATE (State_function)TOP_COUNTING_STATE_HANDLER
#define TOP_DISPLAYING_COUNTER_SET_STATE (State_function) TOP_DISPLAYING_COUNTER_SET_STATE_HANDLER

#endif