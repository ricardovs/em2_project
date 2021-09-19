#ifndef DIAL_HSM
#define DIAL_HSM
#include <ncurses.h>
#include "hsm_header.h"
#include "hsm_event.h"
#define DIAL_WAIT_PERIOD_FAST 1000
#define DIAL_WAIT_PERIOD_SLOW 2500

typedef int (*Get_dial_position)();

typedef struct Dial_hsm{
    Hsm_header header;
    int last_position;
    int elapsed_time;
    int wait_period;
    Get_dial_position get_dial_position;
} Dial_hsm;

//Initial function
void INIT_DIAL_HSM(Dial_hsm* sm, void * event_listener);
void DIAL_EVENT_RECEIVER(Dial_hsm* sm, HsmEvent * const event);

// Hierarchical State Machines
void DIAL_IDLE(Dial_hsm* sm, HsmEvent * const event);
void DIAL_SLOW_MOVEMENT(Dial_hsm* sm, HsmEvent * const event);
    void DIAL_SLOW_WAIT(Dial_hsm* sm, HsmEvent * const event);
void DIAL_FAST_MOVEMENT(Dial_hsm* sm, HsmEvent * const event);
    void DIAL_FAST_WAIT(Dial_hsm* sm, HsmEvent * const event);
void DIAL_STOPPED(Dial_hsm* sm, HsmEvent * const event);


#endif