#include "hsm_header.h"

void INIT_HSM_HEADER(Hsm_header * header, 
                        void * event_handler,
                        void * event_receiver,
                        void * event_dispatcher){
    header->event_handler = event_handler;
    header->event_receiver = event_receiver;
    header->event_dispatcher = event_dispatcher;
    header->pending_event = NO_EVENT;
    return;
}