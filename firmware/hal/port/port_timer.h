#ifndef PORT_TIMER_H
#define PORT_TIMER_H

#include <stdint.h>

#define NUM_TIMERS 10U
/* specific to the ccu6 chip, other wl55 may be different */
#define TIMER1 0x00U
#define TIMER1_CH1 0x00U
#define TIMER1_CH2 0x01U
#define TIMER1_CH3 0x02U
#define TIMER1_CH4 0x03U

#define TIMER2 0x10U
#define TIMER2_CH1 0x10U
#define TIMER2_CH2 0x11U
#define TIMER2_CH3 0x12U
#define TIMER2_CH4 0x13U

#define TIMER3 0x20U
#define TIMER3_CH1 0x20U
#define TIMER3_CH2 0x21U
#define TIMER3_CH3 0x22U
#define TIMER3_CH4 0x23U

#define TIMER4 0x30U
#define TIMER4_CH1 0x30U
#define TIMER4_CH2 0x31U
#define TIMER4_CH3 0x32U
#define TIMER4_CH4 0x33U

#define TIMER6 0x40U

#define TIMER7 0x50U

#define TIMER8 0x60U
#define TIMER8_CH1 0x60U
#define TIMER8_CH2 0x61U
#define TIMER8_CH3 0x62U
#define TIMER8_CH4 0x63U

#define TIMER15 0x70U
#define TIMER15_CH1 0x70U
#define TIMER15_CH2 0x71U

#define TIMER16 0x80U
#define TIMER16_CH1 0x80U

#define TIMER17 0x90U
#define TIMER17_CH1 0x90U

/* STM32 specific */
#define TIMER_CLK_INTERNAL 0x0000U
#define TIMER_CLK_EXTERNAL 0x0007U

#define TIMER_TRGO_RESET 0x0000U
#define TIMER_TRGO_ENABLE 0x0010U
#define TIMER_TRGO_UPDATE 0x0020U
#define TIMER_TRGO_OC1 0x0040U
#define TIMER_TRGO_OC2 0x0050U
#define TIMER_TRGO_OC3 0x0060U
#define TIMER_TRGO_OC4 0x0070U

/* Modes are port specific */
#define TIMER_MODE_PWM 0x0000U
#define TIMER_MODE_OC 0x0100U
#define TIMER_MODE_OC_TOGGLE 0x0200U
#define TIMER_MODE_IC 0x0300U

/* input sensitivity, again port specififc */
#define TIMER_INPUT_RISING 0x0000U
#define TIMER_INPUT_FALLING 0x1000U
#define TIMER_INPUT_BOTH 0x2000U

/* timer interrupts */
#define TIMER_ISR_UPDATE 0x00U
#define TIMER_ISR_CCMP 0x01U

void timer_clear_uif_flag(uint8_t tim);

#endif /* PORT_TIMER_H */
