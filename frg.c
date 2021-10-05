/**
 * @file    frg.c
 * @brief   Frequency Generation for EFM32GG
 * @author  RicardoVS
 * @version 1.0
 *
 * @note    Can use any timer
 *
 * @note    The HFPERCLOCK is used as clock source
 *
 * @note    Pinout and Ports for EFM32GG990
 *
 *  Timer  | Channel |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |
 *   ------|---------|----|----|----|----|----|----|----|----|
 * TIMER0  | CC0     |PA0 |PA0 |PF6 |PD1 |PA0 |PF0 |    |    |
 * TIMER0  | CC1     |PA1 |PA1 |PF7 |PD2 |PC0 |PF1 |    |    |
 * TIMER0  | CC2     |PA2 |PA2 |PF8 |PD3 |PC1 |PF2 |    |    |
 * TIMER1  | CC0     |    |PE10|PB0 |PB7 |PD6 |    |    |    |
 * TIMER1  | CC1     |    |PE11|PB1 |PB8 |PD7 |    |    |    |
 * TIMER1  | CC2     |    |PE12|PB2 |PB11|    |    |    |    |
 * TIMER2  | CC0     |PA8 |PA12|PC8 |    |    |    |    |    |
 * TIMER2  | CC1     |PA9 |PA13|PC9 |    |    |    |    |    |
 * TIMER2  | CC2     |PA10|PA14|PC10|    |    |    |    |    |
 * TIMER3  | CC0     |PE14|PE0 |    |    |    |    |    |    |
 * TIMER3  | CC1     |PE15|PE1 |    |    |    |    |    |    |
 * TIMER3  | CC2     |PA15|PE2 |    |    |    |    |    |    |
 *
 *
 */


#include <stdint.h>

#include "em_device.h"
#include "frg.h"
#include "gpio.h"
#include "pwm.h"

TIMER_TypeDef* FRG_init(const int timer, const int channel, const int location){
    FRG_CONFIG_GPIO(timer, channel, location);
    return FRG_CONFIG_TIMER(timer, channel, location);
}

void FRG_on(TIMER_TypeDef* timer){
    timer->CMD &= ~TIMER_CMD_STOP;
    timer->CMD |= TIMER_CMD_START;
}

void FRG_off(TIMER_TypeDef* timer){
    timer->CMD &= ~TIMER_CMD_START;
    timer->CMD |= TIMER_CMD_STOP;
}

void FRG_toggle(TIMER_TypeDef* timer){
    if(TIMER_IS_RUNNING(timer)){
        FRG_off(timer);
    }else{
        FRG_on(timer);
    }
}

void _FRG_update_top(TIMER_TypeDef* timer, unsigned top){
    unsigned new_top = timer->TOP;
    new_top &= ~LOWER_16BITS_MASK;
    new_top |= (LOWER_16BITS_MASK & top);
    timer->TOP = new_top;
    timer->CNT &= ~LOWER_16BITS_MASK;
}

void _FRG_update_presc(TIMER_TypeDef* timer, unsigned presc){
    unsigned ctrl = timer->CTRL;
    ctrl &= ~_TIMER_CTRL_PRESC_MASK;
    ctrl |= FRG_GET_CTRL_PRESC_DIV(presc);
    timer->CTRL = ctrl;
}

void FRG_change_all(TIMER_TypeDef* timer, const int channel, const unsigned top, const unsigned presc){
    unsigned frg_running = TIMER_IS_RUNNING(timer);
    if(frg_running){
        FRG_off(timer);
    }
    _FRG_update_presc(timer, presc);
    _FRG_update_top(timer, top);
    if(frg_running){
        FRG_on(timer);
    }
}

void FRG_change_top(TIMER_TypeDef* timer, unsigned top){
    if(TIMER_IS_RUNNING(timer)){
        unsigned topb = timer->TOPB;
        topb &= ~LOWER_16BITS_MASK;
        topb |= (LOWER_16BITS_MASK & top);
        timer->TOPB = topb;
        return;
    }
    _FRG_update_top(timer, top);
}

unsigned TIMER_IS_RUNNING(TIMER_TypeDef* timer){
    return TIMER_STATUS_RUNNING & timer->STATUS;
}

TIMER_TypeDef* FRG_CONFIG_TIMER(const int t, const int cc, const int loc){
    /* Configure TIMER*/
    TIMER_TypeDef* timer = FRG_GET_TIMER(t);
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;              // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= FRG_GET_HFPERCLK0_TIMER(t);            // Enable HFPERCLK for TIMERx
    timer->CMD = _TIMER_CMD_RESETVALUE;                          // RESET CMD
    timer->CTRL = (
                   TIMER_CTRL_MODE_DEFAULT|
                   TIMER_CTRL_MODE_UP|
                   TIMER_CTRL_CLKSEL_PRESCHFPERCLK|
                   TIMER_CTRL_RSSCOIST
    );
    timer->CC[cc].CTRL = (
                    TIMER_CC_CTRL_MODE_DEFAULT |                    //Clear register
                    TIMER_CC_CTRL_MODE_OUTPUTCOMPARE |              //Mode OUTPUTCOMPARE
                    TIMER_CC_CTRL_ICEVCTRL_RISING |
                    TIMER_CC_CTRL_ICEDGE_RISING |
                    TIMER_CC_CTRL_COFOA_TOGGLE
    );                                                             //Toggle on Overflow
    timer->CC[cc].CTRL &= ~TIMER_CC_CTRL_COIST;                    //Initial with 0
    timer->ROUTE &= ~_TIMER_ROUTE_MASK;
    timer->ROUTE |= (FRG_GET_TIMER_ROUTE_CC_PEN(cc)|
                    FRG_GET_TIMER_ROUTE_LOCATION_LOC(loc));

    _FRG_update_top(timer, FRG_DEFAULT_TOP);
    _FRG_update_presc(timer, FRG_DEFAULT_PRESC);

    timer->CMD |= TIMER_CMD_START;
    return timer;
}

int FRG_CONFIG_GPIO(const int t, const int cc, const int loc){

    GPIO_P_TypeDef *gpio;
    int mode = -1;
    FRG_GET_GPIO_AND_MODE(t, cc, loc, &gpio, &mode);

    if(mode < 0){
        return -1;   //Couldn't find pin
    }

    /* Enable HFPERCLK */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;
    /* Enable Clock for GPIO */
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
    if(mode < 8){
        gpio->MODEL &= ~GET_GPIO_P_MODE_MASK(mode);
        gpio->MODEL |= GET_GPIO_P_MODE_PUSHPULL(mode);
    }else{
        gpio->MODEH &= ~GET_GPIO_P_MODE_MASK(mode);
        gpio->MODEH |= GET_GPIO_P_MODE_PUSHPULL(mode);
    }

}

TIMER_TypeDef* FRG_GET_TIMER(int t){
    switch (t){
    case 0:
        return TIMER0;
    case 1:
        return TIMER1;
    case 2:
        return TIMER2;
    case 3:
        return TIMER3;
    default:
        return TIMER0;
    }
}

unsigned FRG_GET_HFPERCLK0_TIMER(const int timer){
    switch(timer){
        case 0:
            return CMU_HFPERCLKEN0_TIMER0;
            break;
        case 1:
            return CMU_HFPERCLKEN0_TIMER1;
            break;
        case 2:
            return CMU_HFPERCLKEN0_TIMER2;
            break;
        default:
            return CMU_HFPERCLKEN0_TIMER0;
            break;
    }
}

unsigned FRG_GET_TIMER_ROUTE_CC_PEN(const int channel){
    switch(channel){
    case 0:
        return TIMER_ROUTE_CC0PEN;
    case 1:
        return TIMER_ROUTE_CC1PEN;
    case 2:
        return TIMER_ROUTE_CC2PEN;
    default:
        return TIMER_ROUTE_CC0PEN;
    }
}
unsigned FRG_GET_TIMER_ROUTE_LOCATION_LOC(const int location){
    switch(location){
    case 0:
        return TIMER_ROUTE_LOCATION_LOC0;
    case 1:
        return TIMER_ROUTE_LOCATION_LOC1;
    case 2:
        return TIMER_ROUTE_LOCATION_LOC2;
    case 3:
        return TIMER_ROUTE_LOCATION_LOC3;
    case 4:
        return TIMER_ROUTE_LOCATION_LOC4;
    case 5:
        return TIMER_ROUTE_LOCATION_LOC5;
    default:
        return TIMER_ROUTE_LOCATION_DEFAULT;
    }
}
unsigned FRG_GET_CTRL_PRESC_DIV(unsigned presc){
    switch (presc){
    case 0:
        return TIMER_CTRL_PRESC_DIV1;
    case 1:
        return TIMER_CTRL_PRESC_DIV2;
    case 2:
        return TIMER_CTRL_PRESC_DIV4;
    case 3:
        return TIMER_CTRL_PRESC_DIV8;
    case 4:
        return TIMER_CTRL_PRESC_DIV16;
    case 5:
        return TIMER_CTRL_PRESC_DIV32;
    case 6:
        return TIMER_CTRL_PRESC_DIV64;
    case 7:
        return TIMER_CTRL_PRESC_DIV128;
    case 8:
        return TIMER_CTRL_PRESC_DIV256;
    case 9:
        return TIMER_CTRL_PRESC_DIV512;
    case 10:
        return TIMER_CTRL_PRESC_DIV1024;
    default:
        return TIMER_CTRL_PRESC_DEFAULT;
    }
}

unsigned GET_GPIO_P_MODE_MASK(const int mode){
    switch(mode){
    case 0:
        return _GPIO_P_MODEL_MODE0_MASK;
    case 1:
        return _GPIO_P_MODEL_MODE1_MASK;
    case 2:
        return _GPIO_P_MODEL_MODE2_MASK;
    case 3:
        return _GPIO_P_MODEL_MODE3_MASK;
    case 4:
        return _GPIO_P_MODEL_MODE4_MASK;
    case 5:
        return _GPIO_P_MODEL_MODE5_MASK;
    case 6:
        return _GPIO_P_MODEL_MODE6_MASK;
    case 7:
        return _GPIO_P_MODEL_MODE7_MASK;
    case 8:
        return _GPIO_P_MODEH_MODE8_MASK;
    case 9:
        return _GPIO_P_MODEH_MODE9_MASK;
    case 10:
        return _GPIO_P_MODEH_MODE10_MASK;
    case 11:
        return _GPIO_P_MODEH_MODE11_MASK;
    case 12:
        return _GPIO_P_MODEH_MODE12_MASK;
    case 13:
        return _GPIO_P_MODEH_MODE13_MASK;
    case 14:
        return _GPIO_P_MODEH_MODE14_MASK;
    case 15:
        return _GPIO_P_MODEH_MODE15_MASK;
    default:
        return _GPIO_P_MODEL_MODE0_MASK;
    }
}

unsigned GET_GPIO_P_MODE_PUSHPULL(const int mode){
    switch(mode){
    case 0:
        return GPIO_P_MODEL_MODE0_PUSHPULL;
    case 1:
        return GPIO_P_MODEL_MODE1_PUSHPULL;
    case 2:
        return GPIO_P_MODEL_MODE2_PUSHPULL;
    case 3:
        return GPIO_P_MODEL_MODE3_PUSHPULL;
    case 4:
        return GPIO_P_MODEL_MODE4_PUSHPULL;
    case 5:
        return GPIO_P_MODEL_MODE5_PUSHPULL;
    case 6:
        return GPIO_P_MODEL_MODE6_PUSHPULL;
    case 7:
        return GPIO_P_MODEL_MODE7_PUSHPULL;
    case 8:
        return GPIO_P_MODEH_MODE8_PUSHPULL;
    case 9:
        return GPIO_P_MODEH_MODE9_PUSHPULL;
    case 10:
        return GPIO_P_MODEH_MODE10_PUSHPULL;
    case 11:
        return GPIO_P_MODEH_MODE11_PUSHPULL;
    case 12:
        return GPIO_P_MODEH_MODE12_PUSHPULL;
    case 13:
        return GPIO_P_MODEH_MODE13_PUSHPULL;
    case 14:
        return GPIO_P_MODEH_MODE14_PUSHPULL;
    case 15:
        return GPIO_P_MODEH_MODE15_PUSHPULL;
    default:
        return GPIO_P_MODEL_MODE0_PUSHPULL;
    }
}
/**
 * @brief   FRG_GET_GPIO_AND_MODE
 *
 * @note Maps (TIMERx, CCx, LOCx) into output pin (Pnx)
 *   --------------------------------------------------------
 *         |         |            Location                   |
 *  Timer  | Channel |  0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |
 *   ------|---------|----|----|----|----|----|----|----|----|
 * TIMER0  | CC0     |PA0 |PA0 |PF6 |PD1 |PA0 |PF0 |    |    |
 * TIMER0  | CC1     |PA1 |PA1 |PF7 |PD2 |PC0 |PF1 |    |    |
 * TIMER0  | CC2     |PA2 |PA2 |PF8 |PD3 |PC1 |PF2 |    |    |
 * TIMER1  | CC0     |    |PE10|PB0 |PB7 |PD6 |    |    |    |
 * TIMER1  | CC1     |    |PE11|PB1 |PB8 |PD7 |    |    |    |
 * TIMER1  | CC2     |    |PE12|PB2 |PB11|    |    |    |    |
 * TIMER2  | CC0     |PA8 |PA12|PC8 |    |    |    |    |    |
 * TIMER2  | CC1     |PA9 |PA13|PC9 |    |    |    |    |    |
 * TIMER2  | CC2     |PA10|PA14|PC10|    |    |    |    |    |
 * TIMER3  | CC0     |PE14|PE0 |    |    |    |    |    |    |
 * TIMER3  | CC1     |PE15|PE1 |    |    |    |    |    |    |
 * TIMER3  | CC2     |PA15|PE2 |    |    |    |    |    |    |
 *
 * @param   t Int representing TIMERn as defined in (em_device.h *), where 'n' can be 0,1,2,3
 *
 * @param   cc Int representing TIMERn_CCx as defined in (em_device.h *), where 'x' can be 0,1,2
 *
 * @param   loc Int representing TIMERn_CCx location as defined in (em_device.h *)
 *
 * @param   gpio Pointer to save GPIO port
 *
 * @param   mode Pointer to save GPIO pin number
 */
void FRG_GET_GPIO_AND_MODE(const int t, const int cc, const int loc, GPIO_P_TypeDef **const gpio, int *const mode){
    switch (t){
    case 0:                    //TIMER 0
        switch(cc){
        case 0:                //TIMER0 CC0
            switch (loc){
            case 0:
            case 1:
            case 4:
                *gpio = GPIOA;
                *mode = 0;
                return;
            case 2:
                *gpio = GPIOF;
                *mode = 6;
                return;
            case 3:
                *gpio = GPIOD;
                *mode = 1;
                return;
            case 5:
                *gpio = GPIOF;
                *mode = 0;
                return;
            default:
                break;
            }
        break;
        case 1:                //TIMER0 CC1
            switch(loc){
            case 0:
            case 1:
                *gpio = GPIOA;
                *mode = 1;
                return;
            case 2:
                *gpio = GPIOF;
                *mode = 7;
                return;
            case 3:
                *gpio = GPIOD;
                *mode = 2;
                return;
            case 4:
                *gpio = GPIOC;
                *mode = 0;
                return;
            case 5:
                *gpio = GPIOF;
                *mode = 1;
                return;
            default:
                break;
            }
        break;
        case 2:                //TIMER0 CC2
            switch(loc){
            case 0:
            case 1:
                *gpio = GPIOA;
                *mode = 2;
                return;
            case 2:
                *gpio = GPIOF;
                *mode = 7;
                return;
            case 3:
                *gpio = GPIOD;
                *mode = 3;
                return;
            case 4:
                *gpio = GPIOC;
                *mode = 1;
                return;
            case 5:
                *gpio = GPIOF;
                *mode = 2;
                return;
            default:
                break;
            }
        break;
        default:
            break;
        }
    break;
    case 1:                    //TIMER 1
        switch(cc){
        case 0:                //TIMER 1 CC 0
            switch (loc){
            case 1:
                *gpio = GPIOE;
                *mode = 10;
                return;
            case 2:
                *gpio = GPIOB;
                *mode = 0;
                return;
            case 3:
                *gpio = GPIOB;
                *mode = 7;
                return;
            case 4:
                *gpio = GPIOD;
                *mode = 6;
                return;
            default:
                break;
            }
        break;
        case 1:                //TIMER 1 CC 1
            switch(loc){
            case 1:
                *gpio = GPIOE;
                *mode = 11;
                return;
            case 2:
                *gpio = GPIOB;
                *mode = 1;
                return;
            case 3:
                *gpio = GPIOB;
                *mode = 8;
                return;
            case 4:
                *gpio = GPIOD;
                *mode = 7;
                return;
            default:
                break;
            }
        break;
        case 2:                //TIMER 1 CC 2
            switch (loc){
            case 1:
                *gpio = GPIOE;
                *mode = 12;
                return;
            case 2:
                *gpio = GPIOB;
                *mode = 2;
                return;
            case 3:
                *gpio = GPIOB;
                *mode = 11;
                return;
            default:
                break;
            }
        break;
        default:
            break;
        }
    break;
    case 2:                    //TIMER 2
        switch(cc){
        case 0:                //TIMER 2 CC 0
            switch (loc){
            case 0:
                *gpio = GPIOA;
                *mode = 8;
                return;
            case 1:
                *gpio = GPIOA;
                *mode = 12;
                return;
            case 2:
                *gpio = GPIOC;
                *mode = 8;
                return;
            default:
                break;
            }
        break;
        case 1:                //TIMER 2 CC 1
            switch(loc){
            case 0:
                *gpio = GPIOA;
                *mode = 9;
                return;
            case 1:
                *gpio = GPIOA;
                *mode = 13;
                return;
            case 2:
                *gpio = GPIOC;
                *mode = 9;
                return;
            default:
                break;
            }
        break;
        case 2:                //TIMER 2 CC 2
            switch(loc){
            case 0:
                *gpio = GPIOA;
                *mode = 10;
                return;
            case 1:
                *gpio = GPIOA;
                *mode = 14;
                return;
            case 2:
                *gpio = GPIOC;
                *mode = 10;
                return;
            default:
                break;
            }
        break;
        default:
            break;
        }
    break;
    case 3:                    //TIMER 3
        switch(cc){
        case 0:                //TIMER 3 CC 0
            switch(loc){
            case 0:
                *gpio = GPIOE;
                *mode = 14;
                return;
            case 1:
                *gpio = GPIOE;
                *mode = 0;
                return;
            default:
                break;
            }
        break;
        case 1:                //TIMER 3 CC 1
            switch(loc){
            case 0:
                *gpio = GPIOE;
                *mode = 15;
                return;
            case 1:
                *gpio = GPIOE;
                *mode = 1;
                return;
            default:
                break;
            }
        break;
        case 2:                //TIMER 3 CC 2
            switch(loc){
            case 0:
                *gpio = GPIOA;
                *mode = 15;
                return;
            case 1:
                *gpio = GPIOE;
                *mode = 2;
                return;
            default:
                break;
            }
        break;
        default:
            break;
        }
    break;
    default:
        break;
    }
}
