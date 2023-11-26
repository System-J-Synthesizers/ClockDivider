#include "channels.h"
#include "seven_segment.h"
#include "hal_chip.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "hal_clock.h"
#include "rtos.h"
#include <stdint.h>

#define NUM_DIVISIONS 23U
#define MIN_DIVISION 2U
#define MAX_DIVISION 24U

#define DIVISION_SEL_EVT 1U
#define CHANNEL_SEL_EVT 2U

static os_tcb *sm_task;
static uint32_t sm_stack[STACK_SIZE];
static os_event *sm_evtq[STACK_SIZE];
static os_event division_evt = {DIVISION_SEL_EVT};
static os_event channel_evt = {CHANNEL_SEL_EVT};

static uint8_t const buttons[NUM_BUTTONS] = {DIVISION_BUTTON, CHANNEL_BUTTON};
static uint8_t volatile button_states[NUM_BUTTONS] = {0U, 0U};
static uint8_t volatile last_button_states[NUM_BUTTONS] = {0U, 0U};

static uint8_t division_table[NUM_DIVISIONS];

static volatile uint8_t current_channel = 0U;
static uint8_t volatile channel_divisions[NUM_CHANNELS];

volatile uint8_t get_division(uint8_t channel) {
  return division_table[channel_divisions[channel]];
}

static void state_machine_run(os_event const *const e) {
	uint8_t new_div = 0U;

	switch(e->e_sig) {
		case CHANNEL_SEL_EVT:
			/* 4 channels so AND with 0b00000011 */
			current_channel = (current_channel + 1U) & 0x03U;

			channel_led_set(current_channel);
			/* set the display data to the new channel */
			seven_seg_display(division_table[channel_divisions[current_channel]]);
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
                        seven_seg_display(division_table[new_div]);
		break;

		default:
			/* NOTE: do nothing...for now? */
		break;
	}
}

/**
 * @brief      Run the state machine
 */
static void state_machine_dispatch(os_tcb *task, os_event const *const e) {
  if(e->e_sig == INIT_SIG) {
    /* initialise the divison table */
    for(uint8_t i = 0U; i < NUM_DIVISIONS; i++) {
      division_table[i] = i + MIN_DIVISION;
    }

    /* initialise the channels*/
    current_channel = 0U;
    channel_led_set(current_channel);
    seven_seg_display(MIN_DIVISION);

    for(uint8_t i = 0U; i < NUM_CHANNELS; i++) {
      channel_divisions[i] = 0U;
      channel_set_division(i, division_table[i]);
    }

    /* initialise the buttons */
    gpiob_clock_enable();
    gpio_config_pin(DIVISION_BUTTON, GPIO_MODE_INPUT | GPIO_PULL_UP);
    gpio_config_pin(CHANNEL_BUTTON, GPIO_MODE_INPUT | GPIO_PULL_UP);

    /* initialise button timer */
    tim17_clock_enable();
    timer_init(BUTTON_TIMER, TIMER_CLK_INTERNAL, BUTTON_TIMER_PRESCALER);
    timer_enable_isr(BUTTON_TIMER, TIMER_ISR_UPDATE);
    timer_overflow(BUTTON_TIMER, BUTTON_TIMER_PERIOD);

    NVIC_EnableIRQ(BUTTON_TIMER_IRQ);
    timer_start(BUTTON_TIMER);
  } else {
    state_machine_run(e);
  }
}

/**
 * @brief      Initialise the state machine
 */
void state_machine_init(void) {
  sm_task = os_task_init(state_machine_dispatch, sm_stack, STACK_SIZE, sm_evtq, STACK_SIZE, STATE_MACHINE_PRIORITY);
}

void TIM1_TRG_COM_TIM17_IRQHandler(void) {
  timer_clear_uif_flag(BUTTON_TIMER);

  for(uint8_t i = 0; i < NUM_BUTTONS; i++) {
    button_states[i] = gpio_get(buttons[i]);
    if((button_states[i] == 0U) && (last_button_states[i] == 1U)) {
    	switch(i) {
    		case 0U: /* DIVISION BUTTON */
    			os_evtq_post(sm_task, &division_evt);
    		break;

    		case 1U: /* CHANNEL BUTTON */
    			os_evtq_post(sm_task, &channel_evt);
    		break;

    		default:
    			/* never happens */
    		break; 
    	}
    }
    last_button_states[i] = button_states[i];
  }
} 