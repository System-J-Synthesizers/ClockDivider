#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include "./port/port_gpio.h"

/* pin mode */
#define GPIO_MODE_INPUT		0x0010U
#define GPIO_MODE_OUTPUT	0x0020U
#define GPIO_MODE_ANALOG	0x0030U
/* more modes defined in the port depending on the chip */

/* output resistor - none is default */
#define GPIO_PULL_NONE		0x0000U
#define GPIO_PULL_UP			0x0100U
#define GPIO_PULL_DOWN		0x0200U

/* isr setting */
#define GPIO_INT_NONE			0x0000U
#define GPIO_INT_RISING		0x1000U
#define GPIO_INT_FALLING	0x2000U
#define GPIO_INT_BOTH			0x3000U

void gpio_config_pin(uint8_t pin, uint16_t config);
void gpio_set(uint8_t pin);
void gpio_reset(uint8_t pin);
void gpio_toggle(uint8_t pin);
uint8_t gpio_get(uint8_t pin);

#endif /* HAL_GPIO_H */
