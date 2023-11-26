#include "hal_chip.h"
#include "hal_clock.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "seven_segment.h"
#include "channels.h"
#include "state_machine.h"
#include "rtos.h"
#include <cross_studio_io.h>

void assert_failed(void) {
  debug_printf("!!! HAL ASSERT FAILED !!!\n");
  while(1U) {
  }
}

#define TIMER_CONFIG (TIMER_MODE_OC_TOGGLE | TIMER_CLK_EXTERNAL | TIMER_INPUT_BOTH)

static uint8_t ch_count[NUM_CHANNELS] = {0, 0, 0, 0};
static uint8_t ch_pin[NUM_CHANNELS] = {CH1_CLKOUT_PIN, CH2_CLKOUT_PIN, CH3_CLKOUT_PIN, CH4_CLKOUT_PIN};

static os_tcb *div_task;
static uint32_t div_stack[STACK_SIZE];
static os_event *div_evtq[STACK_SIZE];

#define RISING_EVENT 1U
#define FALLING_EVENT 2U
#define START_EVENT 3U
#define STOP_EVENT 4U

static os_event rising_evt = {RISING_EVENT};
static os_event falling_evt = {FALLING_EVENT};

static os_event start_evt = {START_EVENT};
static os_event stop_evt = {STOP_EVENT};

#define DIV_DISABLED 0U
#define DIV_ENABLED 1U

static uint8_t div_state = DIV_DISABLED;

uint8_t tmp = 1U;

bool first_rising_edge = false;

void divider_dispatch(os_tcb *task, os_event const *const e) {
  if(e->e_sig == INIT_SIG) {
    gpioa_clock_enable();
    gpiob_clock_enable();
    syscfg_clock_enable();

    gpio_config_pin(CH1_CLKOUT_PIN, GPIO_MODE_OUTPUT | GPIO_PULL_DOWN);
    gpio_config_pin(CH2_CLKOUT_PIN, GPIO_MODE_OUTPUT | GPIO_PULL_DOWN);
    gpio_config_pin(CH3_CLKOUT_PIN, GPIO_MODE_OUTPUT | GPIO_PULL_DOWN);
    gpio_config_pin(CH4_CLKOUT_PIN, GPIO_MODE_OUTPUT | GPIO_PULL_DOWN);
   
    gpio_config_pin(CH2_CLKIN_PIN, GPIO_MODE_INPUT | GPIO_PULL_DOWN | GPIO_INT_BOTH);
    gpio_config_pin(CH3_CLKIN_PIN, GPIO_MODE_INPUT | GPIO_PULL_DOWN);
    gpio_config_pin(CH4_CLKIN_PIN, GPIO_MODE_INPUT | GPIO_PULL_DOWN);
    gpio_config_pin(CH1_CLKIN_PIN, GPIO_MODE_INPUT | GPIO_PULL_DOWN);
    
    gpio_config_pin(START_STOP_PIN, GPIO_MODE_INPUT | GPIO_PULL_DOWN | GPIO_INT_BOTH);

  } else {

    switch(e->e_sig) {
      case START_EVENT:
        div_state = DIV_ENABLED;
      break;

      case STOP_EVENT:
        div_state = DIV_DISABLED;
        first_rising_edge = false;
        for(uint8_t i = 0U; i < NUM_CHANNELS; i++) {
          ch_count[i] = get_division(i);
          gpio_reset(ch_pin[i]);
        }
      break;

      case RISING_EVENT:
        if(div_state == DIV_ENABLED) {
          if(first_rising_edge == false) {
            first_rising_edge = true;
          }

          for(uint8_t i = 0U; i < NUM_CHANNELS; i++) {          
            if(ch_count[i] >= get_division(i)) {
              gpio_toggle(ch_pin[i]);
              ch_count[i] = 0U;
            }
            ch_count[i]++;
          }
        }
      break;

      case FALLING_EVENT:
        if((div_state == DIV_ENABLED) && (first_rising_edge == true)) {
          for(uint8_t i = 0U; i < NUM_CHANNELS; i++) {          
            if(ch_count[i] >= get_division(i)) {
              gpio_toggle(ch_pin[i]);
              ch_count[i] = 0U;
            }
            ch_count[i]++;
          }
        }
      break;

      default:
        /* nothing */
      break;

    }
  }
}

void divider_init(void) {
  div_task = os_task_init(divider_dispatch, div_stack, STACK_SIZE, div_evtq, STACK_SIZE, DIVIDER_PRIORITY);
}

void EXTI4_IRQHandler(void) {
  gpio_clear_isr_flag(CH2_CLKIN_PIN);
  
  if(gpio_get(CH2_CLKIN_PIN) == 1U) {
    os_evtq_post(div_task, &rising_evt);
  } else {
    os_evtq_post(div_task, &falling_evt);
  }
}

void EXTI15_10_IRQHandler(void) {
  gpio_clear_isr_flag(START_STOP_PIN);
  
  if(gpio_get(START_STOP_PIN) == 1U) {
    os_evtq_post(div_task, &start_evt);
  } else {
    os_evtq_post(div_task, &stop_evt);
  }

}

void main(void) {
  seven_seg_init();
  channel_leds_init();
  state_machine_init();
  divider_init();

  os_init();

  while(1) {
  	/* wait for interrupt */
  }
}