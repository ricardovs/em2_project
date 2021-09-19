#include <ncurses.h>
#include "hsm_event.h"
#include "dial_hsm.h"
#include "top_hsm.h"

static int dial_position = 0;

int GET_DIAL_POSITION(){
    return dial_position;
}

void UPDATE_DIAL_POSITION(const int p){
    dial_position = p;
}

void get_dial_event(void * hsm, HsmEvent * event){
    printf("Dial dispatched event: %s", GET_EVENT_NAME(*event));
}

int main(){
    Top_hsm top;
    INIT_TOP_HSM(&top, &get_dial_event);
    INIT_DIAL_HSM(&top.dial, top.header.event_receiver);
    UPDATE_DIAL_POSITION(++dial_position);
    TOP_RUN_ALL_HSMS(&top);

    return 0;
}