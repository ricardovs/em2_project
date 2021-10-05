/** ***************************************************************************
 * @file    main.c
 * @brief   Simple LED Blink Demo for EFM32GG_STK3700
 * @version 1.0
******************************************************************************/

#include <stdint.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include <string.h>

#include "em_device.h"
#include "clock_efm32gg.h"
#include "device_interface.h"
#include "top_hsm.h"
#include "timers.h"
#include "led.h"
#include "lcd.h"
#include "quadrature.h"



#define SYSTICKDIVIDER 1000
#define SOFTDIVIDER    1000

void BlinkLED1(void);
void BlinkLED2(void);
void RUN_STEP(void);

static Top_hsm top;

/*****************************************************************************
 * @brief  Main function
 *
 * @note   Using default clock configuration
 *         HFCLK = HFRCO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 */

int main(void) {
    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);


    /*Initiating Machines*/
    LED_Init(LED1|LED2);
    INIT_TOP_HSM(&top, 0);

    /* Configure SysTick */
    SysTick_Config(SystemCoreClock/SYSTICKDIVIDER);    // Every 1 ms
  
    Timers_add(2,BlinkLED1);
    Timers_add(3,BlinkLED2);
    Timers_add(4,RUN_STEP);

    /* Blink loop */
    while (1) {}

}

/*****************************************************************************
 * @brief  SysTick interrupt handler
 *
 * @note   Called every 1/DIVIDER seconds (= 1/1000 = 1 ms)
 */

void SysTick_Handler(void) {
static int counter = 0;             // must be static
    if( counter != 0 ) {
        counter--;
    } else {
        Timers_dispatch();          // Every 1 second
        counter = SOFTDIVIDER-1;
    }
}


void BlinkLED1(void) {
    LED_Toggle(LED1);
}

void BlinkLED2(void) {
    LED_Toggle(LED2);

}

void RUN_STEP(void){
    RUN_TOP_HSM(&top);
}

void DIAL_DEVICE_INIT(){
    Quadrature_Init();
}
int DIAL_DEVICE_GET_POSITION(){
    return Quadrature_GetPosition();
}



void DISPLAY_DEVICE_INIT(){
    LCD_Init();
    DISPLAY_DEVICE_CLEAR();

}

void DISPLAY_DEVICE_WRITE_STRING(char *const s){
    int i = 0;
    DISPLAY_DEVICE_CLEAR();
    if(*(s+i) == '-'){
        LCD_WriteSpecial(LCD_MINUS, LCD_ON);
        i++;
    }
    uint8_t c = LCD_COLLON3;
    for(int p=1; p<7;){
        switch (*(s+i)){
        case ':':
            LCD_WriteSpecial(c, LCD_ON);
            i++;
            switch (c){
            case LCD_COLLON3:
                c = LCD_COLLON5;
                break;
            case LCD_COLLON5:
                c = LCD_COLLON10;
                break;
            default:
                break;
            }
        default:
            LCD_WriteChar(*(s+i),p);
            i++;p++;
            LCD_WriteChar(*(s+i),p);
            i++;p++;
            break;
        }
    }
}

void DISPLAY_DEVICE_CLEAR(){
    LCD_ClearAll();
}