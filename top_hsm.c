#include "top_hsm.h"
#include <ncurses.h>


void INIT_TOP_HSM(Top_hsm * top, void * outside_listener){
    if(outside_listener != 0){
        top->event_dispatcher = (Dispatch_function) outside_listener;
    }else{
        top->event_dispatcher = (Dispatch_function) TOP_EVENT_DISPATCHER;
    }
    top->state = TOP_OFF_STATE;
    top->event = NO_EVENT;
    CLEAR_TIMER_COUNTER(&top->counter);
    CLEAR_TIMER_COUNTER(&top->updater);
    INIT_ALL_TOP_HSMS(top);
}

void INIT_ALL_TOP_HSMS(Top_hsm * top){
    DIAL_INIT_HSM(&top->dial);
    DISPLAY_INIT_HSM(&top->display);
    top->display.timer = &top->counter;
}

void RUN_TOP_HSM(Top_hsm * top){
    RUN_DIAL_HSM(&top->dial);
    top->event = top->dial.event;
    Hsm_state next_state;
    while((next_state = top->state(top)) != 0){
        top->state = (State_function) next_state;
    }
}

State_function TOP_OFF_STATE_HANDLER(Top_hsm * top){
    DISPLAY_RECEIVE_EVENT(&top->display, DISPLAY_OFF_EVENT);
    if(top->event == NO_EVENT){
        return 0;
    }
    return 0; //States not implemented yet
    if((top->event == DIAL_MOVED_FORWARD_EVENT) || (top->event == DIAL_MOVED_BACKWARD_EVENT)){
        return TOP_SETTING_COUNTER_STATE;
    }
    return 0;
}

State_function TOP_SETTING_COUNTER_STATE_HANDLER(Top_hsm * top){
    if(top->event == DIAL_STOPPED_EVENT){
        if(IS_TIME_COUNTER_ZERO(&top->counter)){
            top->state=TOP_OFF_STATE;
            return 0;
        }else{
            return TOP_COUNTING_STATE;
        }
    }
    CLEAR_TIMER_COUNTER(&top->counter);
    DISPLAY_RECEIVE_EVENT(&top->display, DISPLAY_ON_EVENT);
    top->state = TOP_DISPLAYING_COUNTER_SET_STATE;
    return 0;
}

void TOP_EVENT_DISPATCHER(HsmEvent * event){
    return;
}

State_function TOP_DISPLAYING_COUNTER_SET_STATE_HANDLER(Top_hsm * top){
    return 0;
}

State_function TOP_COUNTING_STATE_HANDLER(Top_hsm * top){
    return 0;
}