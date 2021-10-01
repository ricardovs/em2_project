#ifndef DIAL_READER_H
#define DIAL_READER_H

#include "dial_hsm.h"
#include "hsm_event.h"
#include "time_counter_hsm.h"

#define DIAL_READER_DISPLAY_PERIOD 500

typedef enum DialReader_Operation{
    sum,
    sub
}DialReader_Operation;

typedef struct DialReader_hsm{
    State_function state;
    Dial_hsm * dial;
    TimeCounter value;
    TimeCounter *initial_value;
    TimeCounter operand;
    DialReader_Operation operation;
    int remain_time;
}DialReader_hsm;

void DIAL_READER_INIT_HSM(DialReader_hsm *const);
void RUN_DIAL_READER_HSM(DialReader_hsm *const);
void DIAL_READER_UPDATE_OPERAND(DialReader_hsm *const);
void DIAL_READER_UPDATE_VALUE(DialReader_hsm *const);

void DIAL_READER_TRANS(DialReader_hsm *const, const State_function);

State_function DIAL_READER_IDLE_STATE_HANDLER(DialReader_hsm *const);
State_function DIAL_READER_STARTER_STATE_HANDLER(DialReader_hsm *const);
State_function DIAL_READER_READING_STATE_HANDLER(DialReader_hsm *const);
State_function DIAL_READER_DISPLAY_STATE_HANDLER(DialReader_hsm *const);
State_function DIAL_READER_ENDED_READING_STATE_HANDLER(DialReader_hsm *const);

#define DIAL_READER_IDLE_STATE (State_function)DIAL_READER_IDLE_STATE_HANDLER
#define DIAL_READER_STARTER_STATE (State_function)DIAL_READER_STARTER_STATE_HANDLER
#define DIAL_READER_READING_STATE (State_function)DIAL_READER_READING_STATE_HANDLER
#define DIAL_READER_DISPLAY_STATE (State_function)DIAL_READER_DISPLAY_STATE_HANDLER
#define DIAL_READER_ENDED_READING_STATE (State_function)DIAL_READER_ENDED_READING_STATE_HANDLER

#endif