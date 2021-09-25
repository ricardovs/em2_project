#ifndef TIME_COUNTER_HSM_H
#define TIME_COUNTER_HSM_H

#define  ZERO_TIME_COUNTER (TimeCounter){0,0,0,0}
#define  MAX_TIME_COUNTER  (TimeCounter){2,0,0,0}
#define  MIN_TIME_COUNTER  ZERO_TIME_COUNTER

typedef struct TimeCounter{
    int hours;
    int minutes;
    int seconds;
    int miliseconds;
} TimeCounter;


void TIME_COUNTER_INIT(TimeCounter *const sm);
int ADD_TIMER_COUNTER(const TimeCounter *const a, const TimeCounter *const b, TimeCounter *const c);
int SUB_TIMER_COUNTER(const TimeCounter *const a, const TimeCounter *const b, TimeCounter *const c);
int DECREMENT_TO_ZERO_TIMER_COUNTER(TimeCounter * const a);
void CLEAR_TIMER_COUNTER(TimeCounter * const a);
int TIME_COUNTER_IS_ZERO(const TimeCounter *const t);
int TIME_COUNTER_IS_NEGATIVE(const TimeCounter *const t);
int TIME_COUNTER_ADD_SECONDS(TimeCounter *const a, const int v);
int TIME_COUNTER_ADD_MINUTES(TimeCounter *const a, const int v);
int TIME_COUNTER_ADD_HOURS(TimeCounter *const a, const int v);
int TIME_COUNTER_SUB_SECONDS(TimeCounter *const a, const int v);
int TIME_COUNTER_SUB_MINUTES(TimeCounter *const a, const int v);
int TIME_COUNTER_SUB_HOURS(TimeCounter *const a, const int v);
int TIME_COUNTER_EQUALS(const TimeCounter *const a, const TimeCounter *const b);
int TIME_COUNTER_GREATER(const TimeCounter *const a, const TimeCounter *const b);
int TIME_COUNTER_LESS(const TimeCounter *const a, const TimeCounter *const b);
int TIME_COUNTER_GREATER_THAN_MAX(const TimeCounter *const a);
int TIME_COUNTER_LESS_THAN_MIN(const TimeCounter *const a);
void TIME_COUNTER_COPY(TimeCounter *const a, const TimeCounter *const b);
void TIME_COUNTER_TO_STRING(const TimeCounter *const t, char *const s);
#endif