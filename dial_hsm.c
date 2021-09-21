#include<stdio.h>
#include "dial_hsm.h"
#include "utils_hsm.h"

void INIT_DIAL_HSM(Dial_hsm* sm, void * event_listener){
    DIAL_DEVICE_INIT();
    INIT_HSM_HEADER(&sm->header,  &DIAL_IDLE_STATE ,  &dial_event_receiver, event_listener);
    sm->last_sampled_position = DIAL_DEVICE_GET_POSITION();
    sm->last_position = sm->last_sampled_position;
    sm->stopped_time = 0;
    sm->sample_time = 0;
    sm->sample_commands = 0;
}

void dial_event_receiver(const HsmEvent * const event){
    return;
}

void get_diff_and_moved_foward(const unsigned * const new_position, const unsigned * const old_position, int * const diff, int * const moved_foward){
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
void DIAL_IDLE_STATE(Dial_hsm * const sm, HsmEvent * const event){
    int position = DIAL_DEVICE_GET_POSITION();
    sm->header.event_handler = (State_function) &DIAL_IDLE_STATE;
    if (position == sm->last_position){
        return;
    }
    int diff, moved_foward;
    get_diff_and_moved_foward(&position, &sm->last_position, &diff, &moved_foward);
    sm->last_position = position;
    sm->stopped_time = DIAL_PERIOD_STOPPED;
    sm->sample_time = DIAL_PERIOD_SAMPLE;
    sm->sample_commands = 0;
    if(diff > DIAL_FAST_DIFF_POSITION){
        sm->last_sampled_position = position;
        sm->header.event_handler = (State_function) &DIAL_FAST_MOVEMENT_STATE;
        if(moved_foward){
            sm->header.pending_event = FWD_DIAL_FAST_EVENT;
        }else{
            sm->header.pending_event = BWD_DIAL_FAST_EVENT;
        }
        sm->header.event_dispatcher(&sm->header.pending_event);
    }else{
        sm->header.event_handler = (State_function) &DIAL_SLOW_MOVEMENT_STATE;
        if(moved_foward){
            sm->header.pending_event = FWD_DIAL_SLOW_EVENT;
        }else{
            sm->header.pending_event = BWD_DIAL_SLOW_EVENT;
        }
        sm->header.event_dispatcher(&sm->header.pending_event);
    }
}

void DIAL_SLOW_MOVEMENT_STATE(Dial_hsm *const sm, HsmEvent *const event){
    int position = DIAL_DEVICE_GET_POSITION();
    sm->sample_time--;
    sm->stopped_time--;
    sm->header.event_handler = (State_function) &DIAL_SLOW_MOVEMENT_STATE;
    if (position == sm->last_position){
        if(sm->stopped_time > 0){
            if(sm->sample_time <= 0){
                sm->sample_time = DIAL_PERIOD_SAMPLE;
                sm->sample_commands = 0;
                sm->last_sampled_position = position;
            }
            return;
        }
        sm->stopped_time = 0;
        sm->sample_commands = 0;
        sm->header.event_handler = (State_function) &DIAL_IDLE_STATE;
        sm->header.pending_event = DIAL_STOPPED_EVENT;
        sm->header.event_dispatcher(&sm->header.pending_event);
        return;
    }
    sm->stopped_time = DIAL_PERIOD_STOPPED;
    sm->sample_commands++;
    int diff, moved_foward;
    get_diff_and_moved_foward(&position, &sm->last_position, &diff, &moved_foward);
    sm->last_position = position;
    if((sm->sample_commands > DIAL_FAST_SAMPLED_COMANDS)||(diff > DIAL_FAST_DIFF_POSITION)){
        sm->last_sampled_position = position;
        sm->sample_time = DIAL_PERIOD_SAMPLE;
        sm->sample_commands = 0;
        sm->header.event_handler = (State_function) &DIAL_FAST_MOVEMENT_STATE;
        if(moved_foward){
            sm->header.pending_event = FWD_DIAL_FAST_EVENT;
        }else{
            sm->header.pending_event = BWD_DIAL_FAST_EVENT;
        }
        sm->header.event_dispatcher(&sm->header.pending_event);
        return;
    }else{
        int sampled_foward;
        get_diff_and_moved_foward(&position, &sm->last_sampled_position, &diff, &sampled_foward);
        if(diff > DIAL_FAST_DIFF_POSITION){
            sm->header.event_handler = (State_function) &DIAL_FAST_MOVEMENT_STATE;
            sm->last_sampled_position = position;
            sm->sample_time = DIAL_PERIOD_SAMPLE;
            sm->sample_commands = 0;
            if(sampled_foward){
                sm->header.pending_event = FWD_DIAL_FAST_EVENT;
            }else{
                sm->header.pending_event = BWD_DIAL_FAST_EVENT;
            }
            sm->header.event_dispatcher(&sm->header.pending_event);
            return;
        }
        if(sm->sample_time <= 0){
            sm->sample_time = DIAL_PERIOD_SAMPLE;
            sm->last_sampled_position = position;
            sm->sample_commands = 0;
        }
        if(moved_foward){
            sm->header.pending_event = FWD_DIAL_SLOW_EVENT;
        }else{
            sm->header.pending_event = BWD_DIAL_SLOW_EVENT;
        }
        sm->header.event_dispatcher(&sm->header.pending_event);
    }
}

void DIAL_FAST_MOVEMENT_STATE(Dial_hsm* sm, HsmEvent * event){
    int position = DIAL_DEVICE_GET_POSITION();
    sm->sample_time--;
    sm->stopped_time--;
    sm->header.event_handler = (State_function) &DIAL_FAST_MOVEMENT_STATE;
    if (position == sm->last_position){
        if(sm->stopped_time > 0){
            if(sm->sample_time <= 0){
                sm->sample_time = DIAL_PERIOD_SAMPLE;
                sm->sample_commands = 0;
                sm->last_sampled_position = position;
            }
            return;
        }
        sm->stopped_time = 0;
        sm->sample_commands = 0;
        sm->header.event_handler = (State_function) &DIAL_IDLE_STATE;
        sm->header.pending_event = DIAL_STOPPED_EVENT;
        sm->header.event_dispatcher(&sm->header.pending_event);
        return;
    }
    sm->stopped_time = DIAL_PERIOD_STOPPED;
    sm->sample_commands++;
    int diff, moved_foward;
    get_diff_and_moved_foward(&position, &sm->last_position, &diff, &moved_foward);
    sm->last_position = position;
    if(moved_foward){
        sm->header.pending_event = FWD_DIAL_FAST_EVENT;
    }else{
        sm->header.pending_event = BWD_DIAL_FAST_EVENT;
    }
    if(diff > DIAL_FAST_DIFF_POSITION){
        sm->last_sampled_position = position;
        sm->sample_time = DIAL_PERIOD_SAMPLE;
        sm->sample_commands = 0;
        sm->header.event_dispatcher(&sm->header.pending_event);
        return;
    }else{
        if(sm->sample_time > 0){
            sm->header.event_dispatcher(&sm->header.pending_event);
            return;
        }else{
            sm->sample_time = DIAL_PERIOD_SAMPLE;
            sm->last_sampled_position = position;
            sm->sample_commands = 0;
            sm->header.event_handler = (State_function) &DIAL_SLOW_MOVEMENT_STATE;
            if(moved_foward){
                sm->header.pending_event = FWD_DIAL_SLOW_EVENT;
            }else{
                sm->header.pending_event = BWD_DIAL_SLOW_EVENT;
            }
            sm->header.event_dispatcher(&sm->header.pending_event);
        }
    }
}