#ifndef PORT_GPIO_H
#define PORT_GPIO_H

#include <stdint.h>

#define NUM_PORTS 4U
#define PORT_A 0x00U
#define PORT_B 0x10U
#define PORT_F 0x20U
#define PORT_G 0x20U

#define GPIO_MODE_AF0		0x00U
#define GPIO_MODE_AF1		0x01U
#define GPIO_MODE_AF2		0x02U
#define GPIO_MODE_AF3		0x03U
#define GPIO_MODE_AF4		0x04U
#define GPIO_MODE_AF5		0x05U
#define GPIO_MODE_AF6		0x06U
#define GPIO_MODE_AF7		0x07U
#define GPIO_MODE_AF8		0x08U
#define GPIO_MODE_AF9		0x09U
#define GPIO_MODE_AF10  0x0AU	
#define GPIO_MODE_AF11  0x0BU
#define GPIO_MODE_AF12  0x0CU
#define GPIO_MODE_AF13  0x0DU
#define GPIO_MODE_AF14  0x0EU
#define GPIO_MODE_AF15  0x0FU

/* this is port specific hence not being in hal_gpio.h */
void gpio_clear_isr_flag(uint8_t pin_num);

#endif /* PORT_GPIO_H */
