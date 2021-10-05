#ifndef FRG_HSM_H
#define FRG_HSM_H
#include "frg.h"

typedef struct Frg_hsm{
    TIMER_TypeDef* timer;
    int channel;
    int presc;
    unsigned top;
}Frg_hsm;

void INIT_FRG_HSM(Frg_hsm * frg, const int timer, const int channel, const int location);
void UPDATE_TOP_FRG(Frg_hsm * frg, const unsigned top);
void UPDATE_FREQ_FRG(Frg_hsm * frg, const unsigned top, const int presc);
void TOGGLE_FRG(Frg_hsm * frg);
void TURN_ON_FRG(Frg_hsm * frg);
void TURN_OFF_FRG(Frg_hsm * frg);
#endif // FRG_HSM_H
