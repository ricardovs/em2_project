#include "time_counter_hsm.h"

void CLEAR_TIMER_COUNTER(TimeCounter * const a){
    a->hours = 0;
    a->minutes = 0;
    a->seconds = 0;
    a->miliseconds = 0;
}
int DECREMENT_TIMER_COUNTER(TimeCounter * const a){
    if(a->miliseconds > 0){
        a->miliseconds--;
        return 0;
    }else if(a->seconds > 0){
        a->miliseconds = 999;
        a->seconds--;
        return 0;
    }else if(a->minutes > 0){
        a->miliseconds = 999;
        a->seconds = 59;
        a->minutes--;
        return 0;
    }else if(a->hours > 0){
        a->miliseconds = 999;
        a->seconds = 59;
        a->minutes = 59;
        a->hours--;
    }
    return -1;
}

int IS_TIME_COUNTER_ZERO(const TimeCounter *const t){
    if(t->hours > 0){
        return 0;
    }
    if(t->minutes > 0){
        return 0;
    }
    if(t->seconds > 0){
        return 0;
    }
    if(t->miliseconds > 0){
        return 0;
    }
    return 1;
}