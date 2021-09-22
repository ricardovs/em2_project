#ifndef DIAL_HSM_H
#define DIAL_HSM_H
#include "hsm_header.h"
#include "hsm_event.h"
#include "device_interface.h"
// Constants Definition
#define DIAL_PERIOD_STOPPED 2000
#define DIAL_TICKS_PER_ROTATION 20

typedef struct Dial_hsm{
    State_function state;
    HsmEvent event;
    int position;
    int diff;
    int stopped_time;
} Dial_hsm;


//DIAL FUNCTIONS
void DIAL_INIT_HSM(Dial_hsm * sm);
void RUN_DIAL_HSM(Dial_hsm *sm);
void DIAL_TRANS(Dial_hsm *const sm, State_function next_state);

//SCRIPTS
void dial_diff_script(const unsigned * const new_position, const unsigned * const last_sampled_position, int * const diff, int * const moved_foward);

// Hierarchical State Machines
State_function DIAL_STOPPED_STATE_HANDLER(Dial_hsm *const sm);
State_function DIAL_MOVING_FORWARD_STATE_HANDLER(Dial_hsm *const sm);
State_function DIAL_MOVING_BACKWARD_STATE_HANDLER(Dial_hsm *const sm);

#define DIAL_STOPPED_STATE (State_function)DIAL_STOPPED_STATE_HANDLER
#define DIAL_MOVING_FORWARD_STATE (State_function)DIAL_MOVING_FORWARD_STATE_HANDLER
#define DIAL_MOVING_BACKWARD_STATE (State_function)DIAL_MOVING_BACKWARD_STATE_HANDLER

#endif