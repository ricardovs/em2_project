#ifndef HSM_HEADER_H
#define HSM_HEADER_H
#include "hsm_event.h"

typedef void (*Hsm_state)(void * const Hsm_machine);
typedef Hsm_state (*State_function)(void * const Hsm_machine);
typedef void (*Run_function)(void * const Hsm_machine);
typedef void (*Receive_function)(void * const Hsm_machine, const HsmEvent * const event);
typedef void (*Dispatch_function)(const HsmEvent * const event);

#endif