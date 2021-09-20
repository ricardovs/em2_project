#include "top_hsm.h"

void INIT_TOP_HSM(Top_hsm * top, void * outside_listener){
    INIT_HSM_HEADER(&top->header,  &TOP_IDLE_STATE ,  &TOP_EVENT_RECEIVER, outside_listener);
    TOP_INIT_ALL_HSMS(top);
}

void TOP_EVENT_RECEIVER(Top_hsm * top, HsmEvent * event){
    top->header.event_dispatcher(event);
    return;
}

void TOP_INIT_ALL_HSMS(Top_hsm * top){
    INIT_DIAL_HSM(&top->dial, top->header.event_dispatcher);
}

void TOP_RUN_ALL_HSMS(Top_hsm * top){
    top->dial.header.event_handler(&top->dial, &top->dial.header.pending_event);
}

void TOP_IDLE_STATE(Top_hsm * top, HsmEvent * event){
    return;
}