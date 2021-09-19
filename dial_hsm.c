#include "dial_hsm.h"

extern void INIT_HSM_HEADER(Hsm_header * header, 
                     void * event_handler,
                     void * event_receiver,
                     void * event_dispatcher);

extern int GET_DIAL_POSITION();

void INIT_DIAL_HSM(Dial_hsm* sm, void * event_listener){
    INIT_HSM_HEADER(&sm->header,  &DIAL_IDLE ,  &DIAL_EVENT_RECEIVER, event_listener); 
    sm->last_position = GET_DIAL_POSITION();
    sm->elapsed_time = 0;
    sm->wait_period = DIAL_WAIT_PERIOD_SLOW;
}

void DIAL_EVENT_RECEIVER(Dial_hsm* sm, HsmEvent * const event){
    return;
}


// Hierarchical State Machines
void DIAL_IDLE(Dial_hsm* sm, HsmEvent * const event){

}
void DIAL_SLOW_MOVEMENT(Dial_hsm* sm, HsmEvent * const event){

}
void DIAL_SLOW_WAIT(Dial_hsm* sm, HsmEvent * const event){
    
}

void DIAL_FAST_MOVEMENT(Dial_hsm* sm, HsmEvent * const event){

}
void DIAL_FAST_WAIT(Dial_hsm* sm, HsmEvent * const event){

}

void DIAL_STOPPED(Dial_hsm* sm, HsmEvent * const event){

}