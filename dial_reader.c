#include "dial_reader.h"

void DIAL_READER_UPDATE_OPERAND(DialReader_hsm *const dial_reader){
    dial_reader->operand.hours=0;
    dial_reader->operand.minutes=0;
    dial_reader->operand.seconds=0;
    dial_reader->operand.miliseconds=0;
    switch(dial_reader->dial->diff){
        case 0:
            break;
        case 1:
            dial_reader->operand.seconds=1;
            break;
        case 2:
            dial_reader->operand.seconds=2;
            break;
        case 3:
            dial_reader->operand.seconds=5;
            break;
        case 4:
            dial_reader->operand.seconds=15;
            break;
        case 5:
            dial_reader->operand.seconds=30;
            break;
        case 6:
        case 7:
            dial_reader->operand.minutes=1;
            break;
        case 8:
            dial_reader->operand.minutes=2;
            break;
        case 9:
            dial_reader->operand.minutes=5;
            break;
        case 10:
            dial_reader->operand.minutes=10;
            break;
        case 11:
            dial_reader->operand.minutes=20;
            break;
        case 12:
            dial_reader->operand.minutes=30;
            break;
        case 13:
            dial_reader->operand.minutes=40;
            break;
        case 14:
            dial_reader->operand.minutes=50;
            break;
        case 15:
            dial_reader->operand.hours=1;
            break;
        case 16:
            dial_reader->operand.hours=1;
            dial_reader->operand.minutes=15;
            break;
        case 17:
            dial_reader->operand.hours=1;
            dial_reader->operand.minutes=30;
            break;
        case 18:
            dial_reader->operand.hours=1;
            dial_reader->operand.minutes=45;
            break;
        case 19:
            dial_reader->operand.hours=1;
            dial_reader->operand.minutes=55;
            break;
        case 20:
            dial_reader->operand.hours=2;
            break;
        default:
            break;
    }
}

void DIAL_READER_INIT_HSM(DialReader_hsm *const dial_reader){
    dial_reader->state = DIAL_READER_IDLE_STATE;
    dial_reader->dial = 0;
    dial_reader->value = ZERO_TIME_COUNTER;
    dial_reader->initial_value = 0;
    dial_reader->operand = ZERO_TIME_COUNTER;
}

State_function DIAL_READER_IDLE_STATE_HANDLER(DialReader_hsm *const dial_reader){
    if((dial_reader->dial->state == DIAL_MOVING_FORWARD_STATE) ||
        (dial_reader->dial->state == DIAL_MOVING_BACKWARD_STATE)){
            DIAL_READER_TRANS(dial_reader, DIAL_READER_STARTER_STATE);
    }
    return 0;
}

void DIAL_READER_TRANS(DialReader_hsm *const dial_reader, const State_function next_state){
    if(next_state == DIAL_READER_IDLE_STATE){
        DIAL_RESTART_DIFF(dial_reader->dial);
    }else if(next_state == DIAL_READER_DISPLAY_STATE){
        dial_reader->remain_time = DIAL_READER_DISPLAY_PERIOD;
    }
    dial_reader->state = next_state;
}

State_function DIAL_READER_READING_STATE_HANDLER(DialReader_hsm *const dial_reader){
    DIAL_READER_UPDATE_VALUE(dial_reader);
    if(TIME_COUNTER_IS_NEGATIVE(&dial_reader->value)){
        dial_reader->value = ZERO_TIME_COUNTER;
        SUB_TIMER_COUNTER(&dial_reader->value, dial_reader->initial_value, &dial_reader->operand);
        DIAL_RESTART_DIFF(dial_reader->dial);
    }
    else if(TIME_COUNTER_GREATER_THAN_MAX(&dial_reader->value)){
        dial_reader->value = MAX_TIME_COUNTER;
        SUB_TIMER_COUNTER(&dial_reader->value, dial_reader->initial_value, &dial_reader->operand);
        DIAL_RESTART_DIFF(dial_reader->dial);
    }

    if(dial_reader->dial->state == DIAL_STOPPED_STATE){
        DIAL_READER_TRANS(dial_reader, DIAL_READER_DISPLAY_STATE);
    }
    return 0;
}

State_function DIAL_READER_STARTER_STATE_HANDLER(DialReader_hsm *const dial_reader){
    return DIAL_READER_READING_STATE;
}

State_function DIAL_READER_DISPLAY_STATE_HANDLER(DialReader_hsm *const dial_reader){
    dial_reader->remain_time--;
    if(dial_reader->remain_time > 0){
        return 0;
    }
    DIAL_READER_TRANS(dial_reader, DIAL_READER_ENDED_READING_STATE);
    return 0;
}

void DIAL_READER_UPDATE_VALUE(DialReader_hsm *const dial_reader){
    if(dial_reader->dial->diff > 20){
        dial_reader->dial->diff = 20;
    }
    DIAL_READER_UPDATE_OPERAND(dial_reader);
    if(dial_reader->dial->state == DIAL_MOVING_BACKWARD_STATE){
        SUB_TIMER_COUNTER(dial_reader->initial_value, &dial_reader->operand, &dial_reader->value);
    }else{
        ADD_TIMER_COUNTER(dial_reader->initial_value, &dial_reader->operand, &dial_reader->value);
    }
}

void RUN_DIAL_READER_HSM(DialReader_hsm *const dial_reader){
    Hsm_state next_state;
    while((next_state = dial_reader->state(dial_reader)) != 0){
        dial_reader->state = (State_function) next_state;
    }
}


State_function DIAL_READER_ENDED_READING_STATE_HANDLER(DialReader_hsm *const dial_reader){
    DIAL_READER_TRANS(dial_reader,DIAL_READER_IDLE_STATE);
    return 0;
}