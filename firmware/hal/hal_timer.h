#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdint.h>
#include "./port/port_timer.h"

void timer_init(uint8_t tim, uint16_t config, uint16_t prescaler);
void timer_enable_isr(uint8_t tim, uint8_t isr);
void timer_start(uint8_t tim);
void timer_stop(uint8_t tim);

void timer_overflow(uint8_t tim, uint16_t ovf);
void timer_pwm_duty(uint8_t tim, uint16_t duty);
void timer_compare_value(uint8_t tim, uint16_t val);

#endif /* HAL_TIMER_H */
