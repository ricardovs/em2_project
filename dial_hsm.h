#ifndef DIAL_HSM_H
#define DIAL_HSM_H
#include "hsm_header.h"
#include "hsm_event.h"
#include "device_interface.h"
// Constants Definition
#define DIAL_PERIOD_STOPPED 2500
#define DIAL_PERIOD_SAMPLE  500
#define DIAL_FAST_DIFF_POSITION 2
#define DIAL_FAST_SAMPLED_COMANDS 1

typedef struct Dial_hsm{
    Hsm_header header;
    unsigned last_sampled_position;
    unsigned last_position;
    int stopped_time;
    int sample_time;
    int sample_commands;
} Dial_hsm;

//External funcition
extern void INIT_HSM_HEADER(Hsm_header * header,
                     void * event_handler,
                     void * event_receiver,
                     void * event_dispatcher);

//Initial function
void INIT_DIAL_HSM(Dial_hsm * sm, void * event_listener);
void dial_event_receiver(const HsmEvent * const event);
void get_diff_and_moved_foward(const unsigned * const new_position, const unsigned * const last_sampled_position, int * const diff, int * const moved_foward);

// Hierarchical State Machines
void DIAL_IDLE_STATE(Dial_hsm *const sm, HsmEvent *const event);
void DIAL_SLOW_MOVEMENT_STATE(Dial_hsm *const sm, HsmEvent *const event);
void DIAL_FAST_MOVEMENT_STATE(Dial_hsm *const  sm, HsmEvent *const event);

#endif