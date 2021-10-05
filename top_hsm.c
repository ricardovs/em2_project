#include "top_hsm.h"

void INIT_TOP_HSM(Top_hsm * top){
    top->state = TOP_OFF_STATE;
    CLEAR_TIMER_COUNTER(&top->counter);
    top->ticks_to_transit = 0;
    INIT_ALL_TOP_HSMS(top);
}

void INIT_ALL_TOP_HSMS(Top_hsm * top){
    TOP_INIT_DISPLAY(top);
    TOP_INIT_DIAL(top);
    TOP_INIT_DIAL_READER(top);
}

void TOP_INIT_DISPLAY(Top_hsm * top){
    DISPLAY_INIT_HSM(&top->display);
    top->display.timer = &top->counter;
}

void TOP_INIT_DIAL(Top_hsm * top){
    DIAL_INIT_HSM(&top->dial);
}

void TOP_INIT_DIAL_READER(Top_hsm *top){
    DIAL_READER_INIT_HSM(&top->dial_reader);
    top->dial_reader.dial = &top->dial;
}

void RUN_TOP_HSM(Top_hsm * top){
    //Reading Inputs
    RUN_DIAL_HSM(&top->dial);
    RUN_DIAL_READER_HSM(&top->dial_reader);
    //Processing
    Hsm_state next_state;
    while((next_state = top->state(top)) != 0){
        top->state = (State_function) next_state;
    }
    //Showing Results
    RUN_DISPLAY_HSM(&top->display);
}

State_function TOP_OFF_STATE_HANDLER(Top_hsm * top){
    if(top->dial_reader.state == DIAL_READER_IDLE_STATE){
        return 0;
    }
    if(top->dial_reader.state == DIAL_READER_STARTER_STATE){
        TOP_TRANS(top,TOP_SETTING_COUNTER_STATE);
        return 0;
    }
    return 0;
}

State_function TOP_SETTING_COUNTER_STATE_HANDLER(Top_hsm * top){
    DISPLAY_UPDATE(&top->display);
    if(top->dial_reader.state == DIAL_READER_ENDED_READING_STATE){
        top->counter = top->dial_reader.value;
        if(TIME_COUNTER_IS_ZERO(&top->counter)){
            TOP_TRANS(top, TOP_OFF_STATE);
        }else{
            TOP_TRANS(top, TOP_COUNTING_RESET_STATE);
        }
    }else if(top->dial.state == DIAL_WAIT_STATE){
        top->counter = top->dial_reader.value;
        TOP_TRANS(top, TOP_SETTING_COUNTER_WAITING_STATE);
    }
    return 0;
}

State_function TOP_SETTING_COUNTER_WAITING_STATE_HANDLER(Top_hsm * top){
    DISPLAY_UPDATE(&top->display);
    if(top->dial_reader.state == DIAL_READER_ENDED_READING_STATE){
        top->counter = top->dial_reader.value;
        if(TIME_COUNTER_IS_ZERO(&top->counter)){
            TOP_TRANS(top, TOP_OFF_STATE);
        }else{
            TOP_TRANS(top, TOP_COUNTING_RESET_STATE);
        }
    }else if(top->dial.state != DIAL_WAIT_STATE){
        return TOP_SETTING_COUNTER_STATE;
    }
    return 0;
}

State_function TOP_SETTING_UPDATER_STATE_HANDLER(Top_hsm * top){
    DISPLAY_UPDATE_MESSAGE(&top->display);
    if((top->dial_reader.operation == sub) || (TIME_COUNTER_IS_NEGATIVE(&top->dial_reader.operand)) ){
        DISPLAY_ADD_MINUS_TO_MESSAGE(&top->display);
    }
    DISPLAY_UPDATE_DEVICE(&top->display);
    if(top->dial_reader.state == DIAL_READER_ENDED_READING_STATE){
        if(TIME_COUNTER_IS_ZERO(&top->counter)){
            TOP_TRANS(top, TOP_OFF_STATE);
        }else{
            TOP_TRANS(top, TOP_COUNTING_UPDATER_STATE);
        }
    }else if(top->dial.state == DIAL_WAIT_STATE){
        top->counter = top->dial_reader.value;
        TOP_TRANS(top, TOP_SETTING_UPDATER_WAITING_STATE);
    }
    return 0;
}

State_function TOP_SETTING_UPDATER_WAITING_STATE_HANDLER(Top_hsm * top){
    DISPLAY_UPDATE_MESSAGE(&top->display);
    if(top->dial_reader.operation == sub){
        DISPLAY_ADD_MINUS_TO_MESSAGE(&top->display);
    }
    DISPLAY_UPDATE_DEVICE(&top->display);
    if(top->dial_reader.state == DIAL_READER_ENDED_READING_STATE){
        if(TIME_COUNTER_IS_ZERO(&top->counter)){
            TOP_TRANS(top, TOP_OFF_STATE);
        }else{
            TOP_TRANS(top, TOP_COUNTING_UPDATER_STATE);
        }
    }else if(top->dial.state != DIAL_WAIT_STATE){
        return TOP_SETTING_UPDATER_STATE;
    }
    return 0;
}

void TOP_TRANS(Top_hsm *const top, State_function next_state){
    if(next_state == TOP_OFF_STATE){
            DISPLAY_RECEIVE_EVENT(&top->display, DISPLAY_OFF_EVENT);
            CLEAR_TIMER_COUNTER(&top->counter);
            DIAL_READER_TRANS(&top->dial_reader, DIAL_READER_IDLE_STATE);
    }
    else if(next_state == TOP_SETTING_COUNTER_STATE){
        //CLEAR_TIMER_COUNTER(&top->counter);
        top->dial_reader.initial_value = &top->counter;
        top->display.timer = &top->dial_reader.value;
        DIAL_READER_TRANS(&top->dial_reader, DIAL_READER_READING_STATE);
        DISPLAY_RECEIVE_EVENT(&top->display, DISPLAY_ON_EVENT);
    }
    else if(next_state == TOP_SETTING_UPDATER_STATE){
        top->dial_reader.initial_value = &top->counter;
        top->display.timer = &top->dial_reader.operand;
        DIAL_READER_TRANS(&top->dial_reader, DIAL_READER_READING_STATE);
        DISPLAY_RECEIVE_EVENT(&top->display, DISPLAY_ON_EVENT);
    }
    else if(next_state == TOP_COUNTING_STATE){
        top->counter = top->dial_reader.value;
        top->display.timer = &top->counter;
        top->ticks_to_transit = TOP_RESET_STATE_PERIOD;
    }
    else if(next_state == TOP_COUNTING_RESET_STATE){
        top->counter = top->dial_reader.value;
        top->display.timer = &top->counter;
        top->ticks_to_transit = TOP_RESET_STATE_PERIOD;
    }
    else if(next_state == TOP_COUNTING_UPDATER_STATE){
        top->counter = top->dial_reader.value;
        top->display.timer = &top->counter;
    }
    top->state = next_state;
}

State_function TOP_COUNTING_STATE_HANDLER(Top_hsm * top){
    top->ticks_to_transit--;
    if(top->ticks_to_transit > 0){
        return TOP_COUNTING_RESET_STATE;
    }else{
        top->ticks_to_transit=-1;
        return TOP_COUNTING_UPDATER_STATE;
    }
}

State_function TOP_COUNTING_RESET_STATE_HANDLER(Top_hsm * top){
    top->ticks_to_transit--;
    if(top->ticks_to_transit < 0){ //Avoid int overflow
        top->ticks_to_transit = -1;
    }
    DISPLAY_UPDATE_MESSAGE(&top->display);
    if((top->dial_reader.state == DIAL_READER_IDLE_STATE) ||
    (top->dial_reader.state == DIAL_READER_ENDED_READING_STATE)){
        if(DECREMENT_TO_ZERO_TIMER_COUNTER(&top->counter)){
           TOP_TRANS(top, TOP_ENDED_COUNTING_STATE);
        }else{
            if(top->ticks_to_transit <= 0){
                top->state= TOP_COUNTING_UPDATER_STATE;
            }
        }
    }else{
        TOP_TRANS(top, TOP_SETTING_COUNTER_STATE);
    }
    return 0;
}

State_function TOP_COUNTING_UPDATER_STATE_HANDLER(Top_hsm * top){
    if((top->dial_reader.state == DIAL_READER_IDLE_STATE) ||
    (top->dial_reader.state == DIAL_READER_ENDED_READING_STATE)){
        DISPLAY_UPDATE_MESSAGE(&top->display);
        if(DECREMENT_TO_ZERO_TIMER_COUNTER(&top->counter)){
           TOP_TRANS(top, TOP_ENDED_COUNTING_STATE);
        }
    }else{
        TOP_TRANS(top, TOP_SETTING_UPDATER_STATE);
    }
    return 0;
}

State_function TOP_ENDED_COUNTING_STATE_HANDLER(Top_hsm * top){
    //Turn music ON!!!
    TOP_TRANS(top, TOP_OFF_STATE);
    return 0;
}