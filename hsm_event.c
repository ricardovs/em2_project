#include "hsm_event.h"

char* GET_EVENT_NAME(HsmEvent event){
    switch(event){
        case NO_EVENT:
            return "NO_EVENT";
        case ON_DISPLAY:
            return "ON_DISPLAY";
        case OFF_DISPLAY:
            return "OFF_DISPLAY";
        case UPDATE_DISPLAY:
            return "UPDATE_DISPLAY";
        case FWD_DIAL_SLOW:
            return "FWD_DIAL_SLOW";
        case FWD_DIAL_FAST:
            return "FWD_DIAL_FAST";
        case BWD_DIAL_SLOW:
            return "BWD_DIAL_SLOW";
        case BWD_DIAL_FAST:
            return "BWD_DIAL_FAST";
        default:
            return "Invalid Event";
    }
}