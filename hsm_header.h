#ifndef HSM_HEADER
#define HSM_HEADER
#include "hsm_event.h"

typedef void (*Event_function)(HsmEvent * event);
typedef void (*State_function)(void * Hsm_machine, HsmEvent * event);

typedef struct Hsm_header {
    State_function event_handler;
    Event_function event_receiver;
    Event_function event_dispatcher;
    HsmEvent pending_event;
} Hsm_header;

void INIT_HSM_HEADER(Hsm_header * header, 
                     void * event_handler,
                     void * event_receiver,
                     void * event_dispatcher);
#endif