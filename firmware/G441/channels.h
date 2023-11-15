#ifndef CHANNELS_H
#define CHANNELS_H

#include <stdint.h>

#define NUM_CHANNELS 4U
#define CHANNEL_SEL_EVT 0U

void channel_leds_init(void);
void channel_init(uint8_t ch);
void channel_led_set(uint8_t ch);
void channel_led_reset(uint8_t ch);
void channel_set_division(uint8_t ch, uint8_t division);

#endif /* CHANNELS_H */
