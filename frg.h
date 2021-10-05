#ifndef FRG_H
#define FRG_H

#include <stdint.h>
#include "em_device.h"

#define FRG_DEFAULT_TOP 0xFFFF
#define FRG_DEFAULT_PRESC 0x0
#define FRG_DO_TOP  0x58C6 //22726dec
#define FRG_RE_TOP  0x4EE9 //20201dec
#define FRG_MI_TOP  0x4704 //18180dec
#define FRG_FA_TOP  0x4294 //17044dec
#define FRG_SOL_TOP 0x3B2F //15151dec
#define FRG_LA_TOP  0x3543 //13635dec
#define FRG_SI_TOP  0x2F58 //12120dec

#ifndef BIT
#define BIT(N) (1U<<(N))
#endif

#ifndef LOWER_16BITS_MASK
#define LOWER_16BITS_MASK 0x0000FFFF
#endif

#ifndef UPPER_16BITS_MASK
#define UPPER_16BITS_MASK 0xFFFF0000
#endif

TIMER_TypeDef* FRG_init(const int timer, const int channel, const int location);
void FRG_on(TIMER_TypeDef *const timer);
void FRG_off(TIMER_TypeDef*const timer);
void FRG_toggle(TIMER_TypeDef *const timer);
unsigned TIMER_IS_RUNNING(TIMER_TypeDef *const timer);
void FRG_change_all(TIMER_TypeDef*timer, const int channel, const unsigned top, const unsigned presc);
void FRG_change_top(TIMER_TypeDef* timer, const unsigned top);

/* Configuration Helpers */
int FRG_CONFIG_GPIO(const int timer, const int channel, const int location);
TIMER_TypeDef* FRG_CONFIG_TIMER(const int timer, const int channel, const int location);
unsigned FRG_GET_HFPERCLK0_TIMER(const int timer);
unsigned GET_GPIO_P_MODE_MASK(const int mode);
unsigned GET_GPIO_P_MODE_PUSHPULL(const int mode);
unsigned FRG_GET_TIMER_ROUTE_CC_PEN(const int channel);
unsigned FRG_GET_TIMER_ROUTE_LOCATION_LOC(const int location);
unsigned FRG_GET_CTRL_PRESC_DIV(const unsigned presc);
TIMER_TypeDef* FRG_GET_TIMER(const int t);
void FRG_GET_GPIO_AND_MODE(const int t, const int cc, const int loc, GPIO_P_TypeDef **const gpio, int *const mode);

#endif //FRG_H