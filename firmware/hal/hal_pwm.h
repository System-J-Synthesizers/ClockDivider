#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <stdint.h>

void pwm_init(uint16_t ch, uint32_t period_ns);
void pwm_set_duty(uint16_t ch, uint8_t duty);

#endif /* HAL_PWM_H */
