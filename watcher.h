#ifndef WATCHER_H
#define WATCHER_H
#include "top_hsm.h"
#include "dial_hsm.h"
#include "dial_reader.h"
#include "device_interface.h"

typedef struct Watcher{
    Top_hsm ref;
    Top_hsm * mov;
}Watcher;


void WATCHER_RUN_TOP(Watcher *const w);
char const* WATCHER_STRING_TOP_STATE(const Top_hsm *const top);
char const* WATCHER_STRING_DIAL_STATE(const Top_hsm *const top);
char const* WATCHER_STRING_DIAL_READER_STATE(const Top_hsm *const top);
char const* WATCHER_STRING_DISPLAY_STATE(const Top_hsm *const top);


void WATCHER_PRINT_DIFF_TOP_STATE(Watcher *const w);
void WATCHER_PRINT_DIFF_DIAL_STATE(Watcher *const w);
void WATCHER_PRINT_DIFF_DIAL_DIFF_VALUE(Watcher *const w);
void WATCHER_PRINT_DIFF_DIAL_READER_STATE(Watcher *const w);
void WATCHER_PRINT_DIFF_DISPLAY_STATE(Watcher *const w);
void WATCHER_PRINT_DIFF_OPERAND(Watcher *const w);
void WATCHER_PRINT_DIFF_VALUE(Watcher *const w);

#endif

