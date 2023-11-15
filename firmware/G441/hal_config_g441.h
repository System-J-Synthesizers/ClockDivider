#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#include "hal_gpio.h"
#include "hal_timer.h"

/* NOTE: This is WL55 config */

/* channel LED mux select pins */
#define LED_SEL0 (PORT_F | 0U)
#define LED_SEL1 (PORT_F | 1U)

/* 7 segment display segment LEDs */
#define DISP_QA  (PORT_A | 7U)
#define DISP_QB  (PORT_B | 0U)
#define DISP_QC  (PORT_A | 2U)
#define DISP_QD  (PORT_A | 5U)
#define DISP_QE  (PORT_A | 0U)
#define DISP_QF  (PORT_A | 6U)
#define DISP_QG  (PORT_A | 1U)

/* 7 segment display enable pins */
#define TENS_SEL  (PORT_A | 3U)
#define ONES_SEL  (PORT_A | 4U)

/* 
 * Timers for different events:
 * 	SEGMENT_TIMER - Multiplexes the 7 segment LEDs
 *	BUTTON_TIMER - Scans the user input buttons at 50Hz
*/
#define SEGMENT_TIMER TIMER16
#define BUTTON_TIMER TIMER17

/* Button input pins */
#define NUM_BUTTONS 2U
#define CHANNEL_BUTTON  (PORT_B | 6U)
#define DIVISION_BUTTON (PORT_B | 7U)

/* DIN Sync pins */
#define START_STOP_PIN (PORT_A | 10U)
#define CH1_CLKIN_PIN (PORT_A | 11U)  /* TIM4 */
#define CH2_CLKIN_PIN (PORT_B | 4U)   /* TIM3 */
#define CH3_CLKIN_PIN (PORT_A | 15U)  /* TIM2 */
#define CH4_CLKIN_PIN (PORT_A | 8U)   /* TIM1 */
#define CH1_CLKOUT_PIN (PORT_A | 12U) /* TIM4 */
#define CH2_CLKOUT_PIN (PORT_B | 5U)  /* TIM3 */
#define CH3_CLKOUT_PIN (PORT_B | 3U)  /* TIM2 */
#define CH4_CLKOUT_PIN (PORT_A | 9U)  /* TIM1 */

/* DIN Sync timers */
#define CH1_TIMER TIMER4
#define CH2_TIMER TIMER3
#define CH3_TIMER TIMER2
#define CH4_TIMER TIMER1

/* DIN Sync timer channels */
#define CH1_CLKIN TIMER4_CH1
#define CH2_CLKIN TIMER3_CH1
#define CH3_CLKIN TIMER2_CH1
#define CH4_CLKIN TIMER1_CH1

#define CH1_CLKOUT TIMER4_CH2
#define CH2_CLKOUT TIMER3_CH2
#define CH3_CLKOUT TIMER2_CH2
#define CH4_CLKOUT TIMER1_CH2

#endif /* HAL_CONFIG_H */