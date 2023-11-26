#include "hal_clock.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "channels.h"
#include <stdio.h>
#include <cross_studio_io.h>

/**
 * @brief      Set the channel LED that is on
 *
 * @param[in]  ch    Channel number
 */
void channel_led_set(uint8_t ch) {
	switch(ch) {
		case 0U:
			gpio_set(LED_SEL0);
			gpio_set(LED_SEL1);
		break;

		case 1U:
			gpio_reset(LED_SEL0);
		break;

		case 2U:
                        gpio_reset(LED_SEL1);
			gpio_set(LED_SEL0);
		break;

		case 3U:
			gpio_reset(LED_SEL0);
		break;

		default:
		break;
	}
}

/**
 * @brief      Init the LED pins for multiplexer
 */
void channel_leds_init(void) {
	gpiof_clock_enable();
	gpio_config_pin(LED_SEL0, GPIO_MODE_OUTPUT | GPIO_PULL_DOWN);
	gpio_config_pin(LED_SEL1, GPIO_MODE_OUTPUT | GPIO_PULL_DOWN);
}

/**
 * @brief      Initialise a timer channel
 *
 * @param[in]  ch    Channel to init
 */
void channel_init(uint8_t ch) {
	debug_printf("DEBUG: initialising channel %d\n", ch);
}

/**
 * @brief      Set a channels clock division factor
 *
 * @param[in]  ch        Channel to set
 * @param[in]  division  The division
 */
void channel_set_division(uint8_t ch, uint8_t division) {

}

