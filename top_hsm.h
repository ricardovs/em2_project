#ifndef TOP_HSM_H
#define TOP_HSM_H
#include "hsm_header.h"
#include "hsm_event.h"
#include "dial_hsm.h"
#include "display_hsm.h"
#include "dial_reader.h"
#include "time_counter_hsm.h"

#define TOP_RESET_STATE_PERIOD 30000

typedef struct Top_hsm{
    State_function state;
    TimeCounter counter;
    int ticks_to_transit;
    Dial_hsm dial;
    DialReader_hsm dial_reader;
    Display_hsm display;
    Dispatch_function event_dispatcher;
}Top_hsm;

void INIT_TOP_HSM(Top_hsm * top, void * outside_listener);
    void INIT_ALL_TOP_HSMS(Top_hsm * top);
        void TOP_INIT_DISPLAY(Top_hsm * top);
        void TOP_INIT_DIAL(Top_hsm * top);
        void TOP_INIT_DIAL_READER(Top_hsm * top);
void RUN_TOP_HSM(Top_hsm * top);
void TOP_TRANS(Top_hsm * top, State_function next_state);
void TOP_EVENT_DISPATCHER(HsmEvent * event);

State_function TOP_OFF_STATE_HANDLER(Top_hsm * top);
State_function TOP_SETTING_COUNTER_STATE_HANDLER(Top_hsm * top);
    State_function TOP_SETTING_COUNTER_WAITING_STATE_HANDLER(Top_hsm * top);
State_function TOP_COUNTING_STATE_HANDLER(Top_hsm * top);
    State_function TOP_COUNTING_RESET_STATE_HANDLER(Top_hsm * top);
    State_function TOP_COUNTING_UPDATER_STATE_HANDLER(Top_hsm * top);
        State_function TOP_SETTING_UPDATER_STATE_HANDLER(Top_hsm * top);
            State_function TOP_SETTING_UPDATER_WAITING_STATE_HANDLER(Top_hsm * top);
State_function TOP_ENDED_COUNTING_STATE_HANDLER(Top_hsm * top);

#define TOP_OFF_STATE (State_function)TOP_OFF_STATE_HANDLER
#define TOP_SETTING_COUNTER_STATE (State_function)TOP_SETTING_COUNTER_STATE_HANDLER
    #define TOP_SETTING_COUNTER_WAITING_STATE (State_function)TOP_SETTING_COUNTER_WAITING_STATE_HANDLER
#define TOP_COUNTING_STATE (State_function)TOP_COUNTING_STATE_HANDLER
    #define TOP_COUNTING_RESET_STATE (State_function)TOP_COUNTING_RESET_STATE_HANDLER
    #define TOP_COUNTING_UPDATER_STATE (State_function)TOP_COUNTING_UPDATER_STATE_HANDLER
        #define TOP_SETTING_UPDATER_WAITING_STATE (State_function)TOP_SETTING_UPDATER_WAITING_STATE_HANDLER
            #define TOP_SETTING_UPDATER_STATE (State_function)TOP_SETTING_UPDATER_STATE_HANDLER
#define TOP_ENDED_COUNTING_STATE (State_function)TOP_ENDED_COUNTING_STATE_HANDLER
#endif