#include "time_counter_hsm.h"

void CLEAR_TIMER_COUNTER(TimeCounter * const a){
    a->hours = 0;
    a->minutes = 0;
    a->seconds = 0;
    a->miliseconds = 0;
}
int DECREMENT_TO_ZERO_TIMER_COUNTER(TimeCounter * const a){
    if(a->miliseconds > 0){
        a->miliseconds--;
        return 0;
    }
    a->miliseconds = 999;
    if(a->seconds > 0){
        a->seconds--;
        return 0;
    }
    a->seconds = 59;
    if(a->minutes > 0){
        a->minutes--;
        return 0;
    }
    a->minutes = 59;
    if(a->hours > 0){
        a->hours--;
        return 0;
    }
    a->miliseconds = 0;
    a->seconds = 0;
    a->minutes = 0;
    a->hours = 0;
    return -1;
}

int TIME_COUNTER_IS_ZERO(const TimeCounter *const t){
    if(t->hours != 0){
        return 0;
    }
    if(t->minutes != 0){
        return 0;
    }
    if(t->seconds != 0){
        return 0;
    }
    if(t->miliseconds != 0){
        return 0;
    }
    return 1;
}
int TIME_COUNTER_IS_NEGATIVE(const TimeCounter *const t){
    if(t->hours < 0){
        return 1;
    }else if(t->hours > 0){
        return 0;
    }

    if(t->minutes < 0){
        return 1;
    }else if(t->minutes < 0){
        return 0;
    }

    if(t->seconds < 0){
        return 1;
    }else if(t->seconds > 0){
        return 0;
    }

    if(t->miliseconds < 0){
        return 1;
    }
    return 0;
}

void TIME_COUNTER_CORRECT_ELEMENTS_RANGE(TimeCounter *const t){
    if(t->miliseconds > 999){
        do{
            t->seconds++;
            t->miliseconds-=1000;
        }while(t->miliseconds > 999);
    }
    else if(t->miliseconds < -999){
        do{
            t->seconds--;
            t->miliseconds+=1000;
        }while(t->miliseconds < -999);
    }

    if(t->seconds > 59){
        do{
            t->minutes++;
            t->seconds-=60;
        }while(t->seconds > 59);
    }
    else if(t->seconds < -59){
        do{
            t->minutes--;
            t->seconds+=60;
        }while(t->seconds < -59);
    }

    if(t->minutes > 59){
        do{
            t->hours++;
            t->minutes-=60;
        }while(t->minutes > 59);
    }
    else if(t->minutes < -59){
        do{
            t->hours--;
            t->minutes+=60;
        }while(t->minutes < -59);
    }
}
void TIME_COUNTER_CORRECT_REPRESENTATION(TimeCounter *const t){
    TIME_COUNTER_CORRECT_ELEMENTS_RANGE(t);
    if(t->hours == 0){
        if(t->minutes == 0){
            if(t->seconds == 0){
                return;
            }
            else if(t->seconds > 0){
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }else{//t->seconds < 0
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
            }
        }else if(t->minutes > 0){
            if(t->seconds > 0){
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }else if(t->seconds == 0){
                if(t->miliseconds < 0){
                    t->minutes--;
                    t->seconds=59;
                    t->miliseconds+=1000;
                }
                return;
            }else{ // (t->seconds < 0)
                t->minutes--;
                t->seconds+=60;
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }
        }else{//t->minutes < 0
            if(t->seconds == 0){
                if(t->miliseconds > 0){
                    t->minutes++;
                    t->seconds=-59;
                    t->miliseconds-=1000;
                }
                return;
            }else if(t->seconds < 0){
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }else{//t->seconds > 0
                t->minutes++;
                t->seconds-=60;
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }
        }
    }else if(t->hours > 0){
        if(t->minutes == 0){
            if(t->seconds == 0){
                return;
            }
            else if(t->seconds > 0){
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }else{//t->seconds < 0
                t->hours--;
                t->minutes=59;
                t->seconds+=60;
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }
        }else if(t->minutes > 0){
            if(t->seconds > 0){
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }else if(t->seconds == 0){
                if(t->miliseconds < 0){
                    t->minutes--;
                    t->seconds=59;
                    t->miliseconds+=1000;
                }
                return;
            }else{ // (t->seconds < 0)
                t->minutes--;
                t->seconds+=60;
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }
        }else{//t->minutes < 0
            t->hours--;
            t->minutes+=60;
            if(t->seconds == 0){
                if(t->miliseconds < 0){
                    t->minutes--;
                    t->seconds=59;
                    t->miliseconds+=1000;
                }
                return;
            }else if(t->seconds < 0){
                t->minutes--;
                t->seconds+=60;
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }else{//t->seconds > 0
                if(t->miliseconds < 0){
                    t->seconds--;
                    t->miliseconds+=1000;
                }
                return;
            }
        }
    }else{//t->hours < 0
        if(t->minutes == 0){
            if(t->seconds == 0){
                return;
            }
            else if(t->seconds > 0){
                 t->hours++;
                 t->minutes = (-59);
                 t->seconds-=60;
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }else{//t->seconds < 0
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }
        }else if(t->minutes > 0){
            t->hours++;
            t->minutes-=60;
            if(t->seconds < 0){
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }else if(t->seconds == 0){
                if(t->miliseconds > 0){
                    t->minutes++;
                    t->seconds=(-59);
                    t->miliseconds-=1000;
                }
                return;
            }else{ // (t->seconds > 0)
                t->minutes++;
                t->seconds-=60;
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }
        }else{//t->minutes < 0
            if(t->seconds == 0){
                if(t->miliseconds > 0){
                    t->minutes++;
                    t->seconds=(-59);
                    t->miliseconds-=1000;
                }
                return;
            }else if(t->seconds < 0){
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }else{//t->seconds > 0
                t->minutes++;
                t->seconds-=60;
                if(t->miliseconds > 0){
                    t->seconds++;
                    t->miliseconds-=1000;
                }
                return;
            }
        }
    }
    return;
}


int SUB_TIMER_COUNTER(const TimeCounter *const a, const TimeCounter *const b, TimeCounter *const c){
    c->hours = a->hours - b->hours;
    c->minutes = a->minutes - b->minutes;
    c->seconds = a->seconds - b->seconds;
    c->miliseconds = a->miliseconds - b->miliseconds;
    TIME_COUNTER_CORRECT_REPRESENTATION(c);
}

int ADD_TIMER_COUNTER(const TimeCounter *const a, const TimeCounter *const b, TimeCounter *const c){
    c->hours = a->hours + b->hours;
    c->minutes = a->minutes + b->minutes;
    c->seconds = a->seconds + b->seconds;
    c->miliseconds = a->miliseconds + b->miliseconds;
    TIME_COUNTER_CORRECT_REPRESENTATION(c);
}

int TIME_COUNTER_GREATER(const TimeCounter *const a, const TimeCounter *const b){
    if(a->hours > b->hours){
        return 1;
    }else if(a->hours < b->hours){
        return 0;
    }

    if(a->minutes > b->minutes){
        return 1;
    }else if(a->minutes < b->minutes){
        return 0;
    }

    if(a->seconds > b->seconds){
        return 1;
    }else if(a->seconds < b->seconds){
        return 0;
    }

    if(a->miliseconds > b->miliseconds){
        return 1;
    }
    return 0;
}

int TIME_COUNTER_GREATER_THAN_MAX(const TimeCounter *const a){
    TimeCounter t = MAX_TIME_COUNTER;
    return TIME_COUNTER_GREATER(a, &t);
}

void TIME_COUNTER_TO_STRING(const TimeCounter *const t, char *const s){
    return;
}

int TIME_COUNTER_EQUALS(const TimeCounter *const a, const TimeCounter *const b){
    if(a->hours != b->hours){
        return 0;
    }
    if(a->minutes != b->minutes){
        return 0;
    }
    if(a->seconds != b->seconds){
        return 0;
    }
    if(a->miliseconds != b->miliseconds){
        return 0;
    }
    return 1;
}
void TIME_COUNTER_COPY(TimeCounter *const a, const TimeCounter *const b){
    a->hours = b->hours;
    a->minutes = b->minutes;
    a->seconds = b->seconds;
    a->miliseconds = b->miliseconds;
}