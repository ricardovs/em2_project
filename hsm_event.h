#ifndef HSM_EVENT
#define HSM_EVENT

typedef enum HsmEvent{
    NO_EVENT,
    ON_DISPLAY,
    OFF_DISPLAY,
    UPDATE_DISPLAY,
    FWD_DIAL_SLOW,
    FWD_DIAL_FAST,
    BWD_DIAL_SLOW,
    BWD_DIAL_FAST,
    MUSIC_STOP,
    MUSIC_STOP_BEEP,
    MUSIC_START_BEEP,
    MUSIC_START_MI,
    MUSIC_START_DO
} HsmEvent;

char* GET_EVENT_NAME(HsmEvent events);
#endif