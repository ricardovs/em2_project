#ifndef TIME_COUNTER_HSM_H
#define TIME_COUNTER_HSM_H

typedef struct TimeCounter{
    int hours;
    int minutes;
    int seconds;
    int miliseconds;
} TimeCounter;

void TIME_COUNTER_INIT(TimeCounter *const sm);
int ADD_TIMER_COUNTER(const TimeCounter *const a, const TimeCounter *const b, TimeCounter *const c);
int SUB_TIMER_COUNTER(const TimeCounter *const a, const TimeCounter *const b, TimeCounter *const c);
int DECREMENT_TIMER_COUNTER(TimeCounter * const a);
void CLEAR_TIMER_COUNTER(TimeCounter * const a);
int IS_TIME_COUNTER_ZERO(const TimeCounter *const t);
#endif