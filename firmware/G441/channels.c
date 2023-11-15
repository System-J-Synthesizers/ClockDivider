#include "hal_clock.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "channels.h"
#include <stdio.h>

void channel_leds_init(void) {
	gpio_config_pin(LED_SEL0, GPIO_MODE_OUTPUT);
	gpio_config_pin(LED_SEL1, GPIO_MODE_OUTPUT);
}



