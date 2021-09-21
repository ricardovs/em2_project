#include "hsm_event.h"

char* GET_EVENT_NAME(HsmEvent event){
    switch(event){
        case NO_EVENT:
            return "NO_EVENT";
        case ON_DISPLAY_EVENT:
            return "ON_DISPLAY_EVENT";
        case OFF_DISPLAY_EVENT:
            return "OFF_DISPLAY_EVENT";
        case UPDATE_DISPLAY_EVENT:
            return "UPDATE_DISPLAY_EVENT";
        case FWD_DIAL_SLOW_EVENT:
            return "FWD_DIAL_SLOW_EVENT";
        case FWD_DIAL_FAST_EVENT:
            return "FWD_DIAL_FAST_EVENT";
        case BWD_DIAL_SLOW_EVENT:
            return "BWD_DIAL_SLOW_EVENT";
        case BWD_DIAL_FAST_EVENT:
            return "BWD_DIAL_FAST_EVENT";
        case DIAL_STOPPED_EVENT:
            return "DIAL_STOPPED_EVENT";
        default:
            return "Unknonwn Event";
    }
}