#include "watcher.h"
#include "string.h"
#include "utils_hsm.h"

void WATCHER_RUN_TOP(Watcher *const w){
    RUN_TOP_HSM(w->mov);
    WATCHER_PRINT_DIFF_DIAL_STATE(w);
    WATCHER_PRINT_DIFF_DIAL_READER_STATE(w);
    WATCHER_PRINT_DIFF_TOP_STATE(w);
    WATCHER_PRINT_DIFF_DISPLAY_STATE(w);
    WATCHER_PRINT_DIFF_DIAL_DIFF_VALUE(w);
    WATCHER_PRINT_DIFF_OPERAND(w);
    WATCHER_PRINT_DIFF_VALUE(w);
}

char const* WATCHER_STRING_TOP_STATE(const Top_hsm *const top){
    if(top->state == TOP_OFF_STATE){
        return "TOP_OFF_STATE";
    }
    if(top->state == TOP_SETTING_COUNTER_STATE){
        return "TOP_SETTING_COUNTER_STATE";
    }
    if(top->state == TOP_COUNTING_STATE){
        return "TOP_COUNTING_STATE";
    }
    if(top->state == TOP_COUNTING_RESET_STATE){
        return "TOP_COUNTING_RESET_STATE";
    }
    if(top->state == TOP_COUNTING_UPDATER_STATE){
        return "TOP_COUNTING_UPDATER_STATE";
    }
    if(top->state == TOP_SETTING_UPDATER_STATE){
        return "TOP_SETTING_UPDATER_STATE";
    }
    if(top->state == TOP_ENDED_COUNTING_STATE){
        return "TOP_ENDED_COUNTING_STATE";
    }
    return "TOP_UNKONW_STATE";
}
char const* WATCHER_STRING_DIAL_STATE(const Top_hsm *const top){
    if(top->dial.state == DIAL_STOPPED_STATE){
        return "DIAL_STOPPED_STATE";
    }
    if(top->dial.state == DIAL_MOVING_FORWARD_STATE){
        return "DIAL_MOVING_FORWARD_STATE";
    }
    if(top->dial.state == DIAL_MOVING_BACKWARD_STATE){
        return "DIAL_MOVING_BACKWARD_STATE";
    }
    if(top->dial.state == DIAL_WAIT_STATE){
        return "DIAL_WAIT_STATE";
    }
    return "DIAL_UNKONW_STATE";
}
char const* WATCHER_STRING_DIAL_READER_STATE(const Top_hsm *const top){
    if(top->dial_reader.state == DIAL_READER_IDLE_STATE){
        return "DIAL_READER_IDLE_STATE";
    }
    if(top->dial_reader.state == DIAL_READER_STARTER_STATE){
        return "DIAL_READER_STARTER_STATE";
    }
    if(top->dial_reader.state == DIAL_READER_READING_STATE){
        return "DIAL_READER_READING_STATE";
    }
    if(top->dial_reader.state == DIAL_READER_DISPLAY_STATE){
        return "DIAL_READER_DISPLAY_STATE";
    }
    if(top->dial_reader.state == DIAL_READER_ENDED_READING_STATE){
        return "DIAL_READER_ENDED_READING_STATE";
    }
    return "DIAL_READER_UNKNONW_STATE";
}
char const* WATCHER_STRING_DISPLAY_STATE(const Top_hsm *const top){
    if(top->display.state == DISPLAY_ON_STATE){
        return "DISPLAY_ON_STATE";
    }
    if(top->display.state == DISPLAY_OFF_STATE){
        return "DISPLAY_OFF_STATE";
    }
    return "DISPLAY_UNKNONW_STATE";
}


void WATCHER_PRINT_DIFF_TOP_STATE(Watcher *const w){
    if(w->mov->state != w->ref.state){
        OUT_PRINT(WATCHER_STRING_TOP_STATE(&w->ref));
        OUT_PRINT(" --> ");
        OUT_PRINT(WATCHER_STRING_TOP_STATE(w->mov));
        OUT_PRINT("\n");
        w->ref.state = w->mov->state;
    }
}
void WATCHER_PRINT_DIFF_DIAL_DIFF_VALUE(Watcher *const w){
    if(w->mov->dial.diff != w->ref.dial.diff){
        char diff_string[4];
        OUT_PRINT("Diff: ");
        unsigned_int_to_string(diff_string,w->ref.dial.diff,3);
        OUT_PRINT(diff_string);
        OUT_PRINT(" --> ");
        unsigned_int_to_string(diff_string,w->mov->dial.diff,3);
        OUT_PRINT(diff_string);
        OUT_PRINT("\n");
        w->ref.dial.diff = w->mov->dial.diff;
    }
}
void WATCHER_PRINT_DIFF_DIAL_READER_STATE(Watcher *const w){
    if(w->mov->dial_reader.state != w->ref.dial_reader.state){
        OUT_PRINT(WATCHER_STRING_DIAL_READER_STATE(&w->ref));
        OUT_PRINT(" --> ");
        OUT_PRINT(WATCHER_STRING_DIAL_READER_STATE(w->mov));
        OUT_PRINT("\n");
        w->ref.dial_reader.state = w->mov->dial_reader.state;
    }
}

void WATCHER_PRINT_DIFF_DISPLAY_STATE(Watcher *const w){
    if(w->mov->display.state != w->ref.display.state){
        OUT_PRINT(WATCHER_STRING_DISPLAY_STATE(&w->ref));
        OUT_PRINT(" --> ");
        OUT_PRINT(WATCHER_STRING_DISPLAY_STATE(w->mov));
        OUT_PRINT("\n");
        w->ref.display.state = w->mov->display.state;
    }
}

void WATCHER_PRINT_DIFF_DIAL_STATE(Watcher *const w){
    if(w->mov->dial.state != w->ref.dial.state){
        OUT_PRINT(WATCHER_STRING_DIAL_STATE(&w->ref));
        OUT_PRINT(" --> ");
        OUT_PRINT(WATCHER_STRING_DIAL_STATE(w->mov));
        OUT_PRINT("\n");
        w->ref.dial.state = w->mov->dial.state;
    }
}

void WATCHER_PRINT_DIFF_OPERAND(Watcher *const w){
    if(!TIME_COUNTER_EQUALS(&w->mov->dial_reader.operand, &w->ref.dial_reader.operand)){
        OUT_PRINT("Operand: ");
        char s[40];
        DISPLAY_CONVERT_TIME_TO_STRING(s, &w->ref.dial_reader.operand);
        OUT_PRINT(s);
        OUT_PRINT(" --> ");
        DISPLAY_CONVERT_TIME_TO_STRING(s, &w->mov->dial_reader.operand);
        OUT_PRINT(s);
        OUT_PRINT("\n");
        TIME_COUNTER_COPY(&w->ref.dial_reader.operand, &w->mov->dial_reader.operand);
    }
}

void WATCHER_PRINT_DIFF_VALUE(Watcher *const w){
    if(!TIME_COUNTER_EQUALS(&w->mov->dial_reader.value, &w->ref.dial_reader.value)){
        OUT_PRINT("Value: ");
        char s[40];
        DISPLAY_CONVERT_TIME_TO_STRING(s, &w->ref.dial_reader.value);
        OUT_PRINT(s);
        OUT_PRINT(" --> ");
        DISPLAY_CONVERT_TIME_TO_STRING(s, &w->mov->dial_reader.value);
        OUT_PRINT(s);
        OUT_PRINT("\n");
        TIME_COUNTER_COPY(&w->ref.dial_reader.value, &w->mov->dial_reader.value);
    }
}
