#include <stdint.h>
#include "stm32wl55xx.h"
#include "port_gpio.h"
#include "../hal_gpio.h"
#include "../hal_assert.h"

#define GPIO_GET_PORT(p) (((p) & 0xF0U) >> 4U)
#define GPIO_GET_PIN(p) ((p) & 0x0FU)

#define GPIO_GET_MODE(c) ((c) & 0x00FFU)
#define GPIO_GET_PULL(c) ((c) & 0x0F00U) 
#define GPIO_GET_ISRS(c) ((c) & 0xF000U) 

/**
 * Array of available GPIO ports
 */
static GPIO_TypeDef *gpio_ports[NUM_PORTS] = {GPIOA, GPIOB, GPIOC};

/**
 * @brief      Internal helper translates a pin number to EXTICRn register 
 * 							index. See RM0453 11.2.3.
 *
 * @param[in]  pin_num  The pin number
 *
 * @return     Register index 0-3
 */
static uint8_t gpio_get_exti_index(uint8_t pin_num) {
	uint8_t exti_index = 0U;

	switch(pin_num) {
			case 0U:
			case 1U:
			case 2U:
			case 3U:
				exti_index = 0U;
			break;

			case 4U:
			case 5U:
			case 6U:
			case 7U:
				exti_index = 1U;
			break;

			case 8U:
			case 9U:
			case 10U:
			case 11U:
				exti_index = 2U;
			break;

			case 12U:
			case 13U:
			case 14U:
			case 15U:
				exti_index = 3U;
			break;

			default:
				/* do nothing */
			break;
		}

		return exti_index;
}

/**
 * @brief      Internal helper translates a pin number to EXTICR pin offset.
 * 							See RM0453 11.2.3.
 *
 * @param[in]  pin_num  The pin number
 *
 * @return     Pin offset 0-3
 */
static uint8_t gpio_get_exti_pin_num(uint8_t pin_num) {
	uint8_t new_num = pin_num;

	switch(pin_num) {
			case 0U:
			case 1U:
			case 2U:
			case 3U:
				new_num = pin_num;
			break;

			case 4U:
			case 5U:
			case 6U:
			case 7U:
				new_num = pin_num - 4U;
			break;

			case 8U:
			case 9U:
			case 10U:
			case 11U:
				new_num = pin_num - 8U;
			break;

			case 12U:
			case 13U:
			case 14U:
			case 15U:
				new_num = pin_num - 12U;
			break;

			default:
				/* do nothing */
			break;
		}

		return new_num;
}

/**
 * @brief      Enables EXTI interrupt in the NVIC
 *
 * @param[in]  pin_num  The pin number
 */
static void gpio_nvic_enable(uint8_t pin_num) {
	switch(pin_num) {
		case 0U:
			NVIC_EnableIRQ(EXTI0_IRQn);
		break;

		case 1U:
			NVIC_EnableIRQ(EXTI1_IRQn);
		break;

		case 2U:
			NVIC_EnableIRQ(EXTI2_IRQn);
		break;

		case 3U:
			NVIC_EnableIRQ(EXTI3_IRQn);
		break;

		case 4U:
			NVIC_EnableIRQ(EXTI4_IRQn);
		break;

		case 5U:
		case 6U:
		case 7U:
		case 8U:
		case 9U:
			NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;

		case 10U:
		case 11U:
		case 12U:
		case 13U:
		case 14U:
		case 15U:
			NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;

		default:
			/* do nothing */
		break;
	}
}

/**
 * @brief      Configures a given pin
 *
 * @param[in]  pin     The pin (PORT | PIN)
 * @param[in]  config  The configuration. See hal/port_gpio.h.
 */
void gpio_config_pin(uint8_t pin, uint16_t config) {
	HAL_ASSERT(GPIO_GET_PORT(pin) < NUM_PORTS);
	uint8_t port_num = GPIO_GET_PORT(pin);
	uint8_t pin_num = GPIO_GET_PIN(pin);

	uint8_t exti_index = gpio_get_exti_index(pin_num);
	uint8_t exti_pin = gpio_get_exti_pin_num(pin_num);

	switch(GPIO_GET_MODE(config)) {
		case GPIO_MODE_AF0:
		case GPIO_MODE_AF1:
		case GPIO_MODE_AF2:
		case GPIO_MODE_AF3:
		case GPIO_MODE_AF4:
		case GPIO_MODE_AF5:
		case GPIO_MODE_AF6:
		case GPIO_MODE_AF7:
		case GPIO_MODE_AF8:
		case GPIO_MODE_AF9:
		case GPIO_MODE_AF10:	
		case GPIO_MODE_AF11:
		case GPIO_MODE_AF12:
		case GPIO_MODE_AF13:
		case GPIO_MODE_AF14:
		case GPIO_MODE_AF15:
			gpio_ports[port_num]->MODER &= ~(3U << (pin_num*2U));
			gpio_ports[port_num]->MODER |= (2U << (pin_num*2U));

			if(pin_num < 8U) {
				gpio_ports[port_num]->AFR[0U] |= (GPIO_GET_MODE(config) << (pin_num*4U));
			} else {
				gpio_ports[port_num]->AFR[1U] |= (GPIO_GET_MODE(config) << (pin_num*4U));
			}
		break;

		case GPIO_MODE_INPUT:
			gpio_ports[port_num]->MODER &= ~(3U << (pin_num*2U));
		break;

		case GPIO_MODE_ANALOG:
			gpio_ports[port_num]->MODER |= (3U << (pin_num*2U));
		break;

		case GPIO_MODE_OUTPUT:
		default: /* default to output mode if mode is not valid */
			gpio_ports[port_num]->MODER &= ~(3U << (pin_num*2U));
			gpio_ports[port_num]->MODER |= (1U << (pin_num*2U));
		break;
	}

	switch(GPIO_GET_PULL(config)) {
		case GPIO_PULL_UP:
			gpio_ports[port_num]->PUPDR |= (1U << (pin_num*2U));
		break;

		case GPIO_PULL_DOWN:
			gpio_ports[port_num]->PUPDR |= (2U << (pin_num*2U));
		break;

		case GPIO_PULL_NONE:
		default:
			gpio_ports[port_num]->PUPDR &= ~(3U << (pin_num*2U));
		break;
	}

	switch(GPIO_GET_ISRS(config)) {
		case GPIO_INT_RISING:
			SYSCFG->EXTICR[exti_index] |= ((port_num << 1U) << (exti_pin * 4U));
			EXTI->RTSR1 |= (1U << pin_num);
			EXTI->IMR1 |= (1U << pin_num);
			gpio_nvic_enable(pin_num);
		break;

		case GPIO_INT_FALLING:
			SYSCFG->EXTICR[exti_index] |= ((port_num << 1U) << (exti_pin * 4U));
			EXTI->FTSR1 |= (1U << pin_num);
			EXTI->IMR1 |= (1U << pin_num);
			gpio_nvic_enable(pin_num);
		break;

		case GPIO_INT_BOTH:
			SYSCFG->EXTICR[exti_index] |= ((port_num << 1U) << (exti_pin * 4U));
			EXTI->RTSR1 |= (1U << pin_num);
			EXTI->FTSR1 |= (1U << pin_num);
			EXTI->IMR1 |= (1U << pin_num);
			gpio_nvic_enable(pin_num);
		break;

		case GPIO_INT_NONE:
		default:
			/* do nothing */
		break;
	}
}

/**
 * @brief      Set a pin to high
 *
 * @param[in]  pin   The pin
 */
void gpio_set(uint8_t pin) {
	uint8_t port_num = GPIO_GET_PORT(pin);
	uint8_t pin_num = GPIO_GET_PIN(pin);
	gpio_ports[port_num]->BSRR |= (1U << pin_num);
}

/**
 * @brief      Reset a pin to low
 *
 * @param[in]  pin   The pin
 */
void gpio_reset(uint8_t pin) {
	uint8_t port_num = GPIO_GET_PORT(pin);
	uint8_t pin_num = GPIO_GET_PIN(pin);
	gpio_ports[port_num]->BSRR |= (1U << (pin_num + 16U));
}

/**
 * @brief      Toggle a pin
 *
 * @param[in]  pin   The pin
 */
void gpio_toggle(uint8_t pin) {
	uint8_t port_num = GPIO_GET_PORT(pin);
	uint8_t pin_num = GPIO_GET_PIN(pin);
	gpio_ports[port_num]->ODR ^= (1U << pin_num);
}

/**
 * @brief      Read the state of a pin
 *
 * @param[in]  pin   The pin
 *
 * @return     State of the pin (1 or 0)
 */
uint8_t gpio_get(uint8_t pin) {
	uint8_t port_num = GPIO_GET_PORT(pin);
	uint8_t pin_num = GPIO_GET_PIN(pin);
	return ((gpio_ports[port_num]->IDR & (1U << pin_num)) == (1U << pin_num));
}

/**
 * @brief      Clear the interrupt pending flag in the EXTI
 *
 * @param[in]  pin_num  The pin number
 */
void gpio_clear_isr_flag(uint8_t pin_num) {
	EXTI->PR1 |= (1U << pin_num);
}