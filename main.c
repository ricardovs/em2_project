#define SIMULATION_FLAG

//#include <ncurses.h>
#include "hsm_event.h"
#include "dial_hsm.h"
#include "top_hsm.h"
#include "utils_hsm.h"
#include <stdio.h>

static unsigned dial_position = 0;

int DIAL_DEVICE_GET_POSITION(){
    return dial_position;
}

void DIAL_DEVICE_INIT(){
    return;
}

void UPDATE_DIAL_POSITION(const unsigned p){
    dial_position = p;
}

void print_event(HsmEvent * event){
    printf("Dial dispatched event: %s\n.", GET_EVENT_NAME(*event));
}

int main(){
    Top_hsm top;
    INIT_TOP_HSM(&top, &print_event);
    UPDATE_DIAL_POSITION(0xffffffff);
    UPDATE_DIAL_POSITION(dial_position+1);
    TOP_RUN_ALL_HSMS(&top);
    UPDATE_DIAL_POSITION(dial_position+2);
    TOP_RUN_ALL_HSMS(&top);
    UPDATE_DIAL_POSITION(dial_position-1);
    TOP_RUN_ALL_HSMS(&top);
    /*int a = 0xff00ffff;
    printf("a = %d, %u, %#8x\n", a, a, a);
    int b = 0x800ffffff;
    printf("b = %d, %u, %#8x \n", b, b, b);
    unsigned r = catch_sum_overflow(&a, &b);
    printf("Got =  %d\n", r);
    */
    printf("Done.\n");
    return 0;
}