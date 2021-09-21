#ifndef TOP_HSM_H
#define TOP_HSM_H

#include "hsm_event.h"
#include "dial_hsm.h"

typedef struct Top_hsm{
    Hsm_header header;
    Dial_hsm dial;
}Top_hsm;

void INIT_TOP_HSM(Top_hsm * top, void * outside_listener);
void TOP_EVENT_RECEIVER(Top_hsm* top, HsmEvent * event);
void TOP_INIT_ALL_HSMS(Top_hsm * top);
void TOP_RUN_ALL_HSMS(Top_hsm * top);
void TOP_EVENT_DISPATCHER(HsmEvent * event); //Lista com as maquinas que ouvirao eventos

void TOP_IDLE_STATE(Top_hsm * top, HsmEvent * event);
#endif