#include "top_hsm.h"

void INIT_TOP_HSM(Top_hsm * top, void * outside_listener){
    INIT_HSM_HEADER(&top->header,  &TOP_IDLE ,  &TOP_EVENT_RECEIVER, outside_listener);
}

void TOP_EVENT_RECEIVER(Top_hsm * top, HsmEvent * const event){
    top->header.event_dispatcher(top, event);
}
void TOP_RUN_ALL_HSMS(Top_hsm * top){
    top->dial.header.event_handler(&top->dial, &top->dial.header.pending_event);
}

void TOP_IDLE(Top_hsm * top, HsmEvent * const event){
    return;
}