#include "frg_hsm.h"

void INIT_FRG_HSM(Frg_hsm * frg, const int timer, const int channel, const int location){
    frg->timer = FRG_init(timer, channel, location);
    frg->channel = channel;
    frg->top = FRG_DEFAULT_TOP;
    frg->presc = FRG_DEFAULT_PRESC;
}

void UPDATE_TOP_FRG(Frg_hsm * frg, const unsigned top){
    if(frg->top == top){
        return;
    }
    FRG_change_top(frg->timer, top);

}

void UPDATE_FREQ_FRG(Frg_hsm * frg, const unsigned top, const int presc){
    if((frg->top == top) && (frg->presc == presc)){
        return;
    }
    FRG_change_all(frg->timer, frg->channel, top, presc);
}

void TOGGLE_FRG(Frg_hsm * frg){
    FRG_toggle(frg->timer);
}

void TURN_ON_FRG(Frg_hsm * frg){
    FRG_on(frg->timer);
}

void TURN_OFF_FRG(Frg_hsm * frg){
    FRG_off(frg->timer);
}