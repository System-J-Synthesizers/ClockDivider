#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#include "hal_gpio.h"
#include "hal_timer.h"
#include "stm32wl55xx.h"

/* NOTE: This is WL55 config */

/* channel LED mux select pins */
#define LED_SEL0 (PORT_B | 15U)
#define LED_SEL1 (PORT_B | 9U)

/* 7 segment display segment LEDs */
#define DISP_QA  (PORT_C | 2U)
#define DISP_QB  (PORT_C | 1U)
#define DISP_QC  (PORT_A | 7U)
#define DISP_QD  (PORT_A | 4U)
#define DISP_QE  (PORT_A | 9U)
#define DISP_QF  (PORT_A | 5U)
#define DISP_QG  (PORT_A | 6U)

/* 7 segment display enable pins */
#define TENS_SEL  (PORT_A | 11U)
#define ONES_SEL  (PORT_A | 12U)

/* 
 * Timers for different events:
 * 	SEGMENT_TIMER - Multiplexes the 7 segment LEDs
 *	BUTTON_TIMER - Scans the user input buttons at 50Hz
*/
#define SEGMENT_TIMER TIMER16
#define BUTTON_TIMER TIMER17

/* Timer settings (prescale / period etc) */
#define SEGMENT_TIMER_PRESCALER 4U /* 4MHz / 4 == 1MHz */
#define SEGMENT_TIMER_PERIOD 1500U /* (1/1MHz) * 1500 == 666Hz */
#define SEGMENT_TIMER_IRQ TIM16_IRQn

#define BUTTON_TIMER_PRESCALER 4U /* 4MHz / 4 == 1MHz */
#define BUTTON_TIMER_PERIOD 50000U /* (1/1MHz) * 50000 == 50Hz */
#define BUTTON_TIMER_IRQ TIM17_IRQn

/* Button input pins */
#define NUM_BUTTONS 2U
#define DIVISION_BUTTON (PORT_B | 7U)
#define CHANNEL_BUTTON 	(PORT_B | 6U)

#endif /* HAL_CONFIG_H */
