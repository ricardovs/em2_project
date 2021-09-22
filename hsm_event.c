#include "hsm_event.h"

char* GET_EVENT_NAME(HsmEvent event){
    switch(event){
        case NO_EVENT:
            return "NO_EVENT";
        case DISPLAY_ON_EVENT:
            return "DISPLAY_ON_EVENT";
        case DISPLAY_OFF_EVENT:
            return "DISPLAY_OFF_EVENT";
        case DISPLAY_UPDATE_EVENT:
            return "DISPLAY_UPDATE_EVENT";
        case DIAL_STOPPED_EVENT:
            return "DIAL_STOPPED_EVENT";
        case DIAL_MOVED_FORWARD_EVENT:
            return "DIAL_MOVED_FORWARD_EVENT";
        case DIAL_MOVED_BACKWARD_EVENT:
            return "DIAL_MOVED_BACKWARD_EVENT";
        default:
            return "Unknonwn Event";
    }
}