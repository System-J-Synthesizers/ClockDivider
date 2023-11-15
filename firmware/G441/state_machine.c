#include "channels.h"
#include "seven_segment.h"
#include <stdint.h>

#define NUM_DIVISIONS 22U
#define MIN_DIVISION 2U
#define MAX_DIVISION 24U

#define DIVISION_SEL_EVT 1U

static uint8_t division_table[NUM_DIVISIONS];

static volatile uint8_t current_channel = 0U;
static uint8_t volatile channel_divisions[NUM_CHANNELS];

/**
 * @brief      Initialise the state machine
 */
void state_machine_init(void) {
	/* initialise the divison table */
  for(uint8_t i = 0U; i < NUM_DIVISIONS; i++) {
    division_table[i] = i + MIN_DIVISION;
  }

  /* initialise the channels*/
  current_channel = 0U;

  for(uint8_t i = 0U; i < NUM_CHANNELS; i++) {
    channel_divisions[i] = MIN_DIVISION;
  }
}

/**
 * @brief      Run the state machine
 */
void state_machine_run(uint8_t evt) {
	uint8_t new_div = 0U;

	switch(evt) {
		case CHANNEL_SEL_EVT:
			channel_led_reset(current_channel);

			/* 4 channels so AND with 0b00000011 */
			current_channel = (current_channel + 1U) & 0x03U;

			channel_led_set(current_channel);
			/* set the display data to the new channel */
			seven_seg_display(channel_divisions[current_channel]);
		break;

		case DIVISION_SEL_EVT:
			new_div = channel_divisions[current_channel];
			if((new_div + 1U) == NUM_DIVISIONS) {
				new_div = 0U;
			} else {
				new_div++;
			}
			channel_divisions[current_channel] = new_div;
			channel_set_division(current_channel, division_table[new_div]);
		break;

		default:
			/* NOTE: do nothing...for now? */
		break;
	}
}