#ifndef HSM_HEADER_H
#define HSM_HEADER_H
#include "hsm_event.h"

typedef void (*State_function)(void * const Hsm_machine, HsmEvent * const event);
typedef void (*Receive_function)(void * const Hsm_machine, const HsmEvent * const event);
typedef void (*Dispatch_function)(const HsmEvent * const event);



typedef struct Hsm_header {
    State_function event_handler;
    Receive_function event_receiver;
    Dispatch_function event_dispatcher;
    HsmEvent pending_event;
} Hsm_header;

void INIT_HSM_HEADER(Hsm_header * header, 
                     void * event_handler,
                     void * event_receiver,
                     void * event_dispatcher);
#endif