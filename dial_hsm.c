#include <ncurses.h>
#include "dial_hsm.h"
#include "utils_hsm.h"

void RUN_DIAL_HSM(Dial_hsm *dial){
    dial->state(dial);
}

void DIAL_TRANS(Dial_hsm *const dial , State_function next_state){
    if(next_state ==  DIAL_STOPPED_STATE){
        dial->state = DIAL_STOPPED_STATE;
        dial->event = DIAL_STOPPED_EVENT;
        return;
    }
    if(next_state == DIAL_MOVING_FORWARD_STATE){
        dial->state = DIAL_MOVING_FORWARD_STATE;
        dial->stopped_time = DIAL_PERIOD_STOPPED;
        dial->event = DIAL_MOVED_FORWARD_EVENT;
        return;
    }
    if(next_state ==  DIAL_MOVING_BACKWARD_STATE){
        dial->state = DIAL_MOVING_BACKWARD_STATE;
        dial->stopped_time = DIAL_PERIOD_STOPPED;
        dial->event = DIAL_MOVED_BACKWARD_EVENT;
        return;
    }
    dial->state = DIAL_STOPPED_STATE;
    dial->event = NO_EVENT;
}

void DIAL_INIT_HSM(Dial_hsm* dial){
    DIAL_DEVICE_INIT();
    dial->event = NO_EVENT;
    dial->position = 0;
    dial->diff = 0;
    dial->stopped_time = 0;
    dial->state = DIAL_STOPPED_STATE;
}

void DIAL_EVENT_RECEIVER(const HsmEvent * const event){
    return;
}

void dial_diff_script(const unsigned * const new_position, const unsigned * const old_position, int * const diff, int * const moved_foward){
    *diff = *new_position - *old_position;
    if(*diff < 0){
        *diff = (-*diff);
    }
    *moved_foward = (int)(*new_position) > (int)(*old_position);
    if (!(*moved_foward)){
        *moved_foward = catch_sum_overflow(old_position, diff);
    }
}
// Hierarchical State Machines
State_function DIAL_STOPPED_STATE_HANDLER(Dial_hsm * const dial){
    int position = DIAL_DEVICE_GET_POSITION();
    if (position == dial->position){
            dial->event = NO_EVENT;
        return 0;
    }
    int diff, moved_forward;
    dial_diff_script(&position, &dial->position, &dial->diff, &moved_forward);
    dial->position = position;
    if(moved_forward > 0){
        DIAL_TRANS(dial, DIAL_MOVING_FORWARD_STATE);
    }else{
        DIAL_TRANS(dial, DIAL_MOVING_BACKWARD_STATE);
    }
    return 0;
}

State_function DIAL_MOVING_FORWARD_STATE_HANDLER(Dial_hsm *const dial){
    int position = DIAL_DEVICE_GET_POSITION();
    dial->stopped_time--;
    if(position == dial->position){
        if(dial->stopped_time > 0){
            dial->event = NO_EVENT;
            return 0;
        }
        DIAL_TRANS(dial, DIAL_STOPPED_STATE);
        return 0;
    }
    dial->stopped_time = DIAL_PERIOD_STOPPED;
    int diff_detected, moved_forward;
    dial_diff_script(&position, &dial->position, &diff_detected, &moved_forward);
    dial->position = position;
    if(moved_forward){
        if(!catch_sum_overflow(&dial->diff, &diff_detected)){
            dial->diff += diff_detected;
        }else{
            //God knows!!! How many ticks!!
        }
        dial->event = DIAL_MOVED_FORWARD_EVENT;
        return 0;
    }
    if(diff_detected > dial->diff){
        dial->diff = diff_detected - dial->diff;
        DIAL_TRANS(dial, DIAL_MOVING_BACKWARD_STATE);
        return 0;
    }
    dial->diff -= diff_detected;
    dial->event = DIAL_MOVED_BACKWARD_EVENT;
    return 0;
}

State_function DIAL_MOVING_BACKWARD_STATE_HANDLER(Dial_hsm *const dial){
    int position = DIAL_DEVICE_GET_POSITION();
    dial->stopped_time--;
    if(position == dial->position){
        if(dial->stopped_time > 0){
            dial->event = NO_EVENT;
            return 0;
        }
        DIAL_TRANS(dial, DIAL_STOPPED_STATE);
        return 0;
    }
    dial->stopped_time = DIAL_PERIOD_STOPPED;
    int diff_detected, moved_forward;
    dial_diff_script(&position, &dial->position, &diff_detected, &moved_forward);
    dial->position = position;
    if(!moved_forward){
        if(!catch_sum_overflow(&dial->diff, &diff_detected)){
            dial->diff += diff_detected;
        }else{
            //God knows!!! How many ticks!!
        }
        dial->event = DIAL_MOVED_BACKWARD_EVENT;
        return 0;
    }
    if(diff_detected > dial->diff){
        dial->diff = diff_detected - dial->diff;
        DIAL_TRANS(dial, DIAL_MOVING_FORWARD_STATE);
        return 0;
    }
    dial->diff -= diff_detected;
    dial->event = DIAL_MOVED_FORWARD_EVENT;
    return 0;
}