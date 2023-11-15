#include "hal_chip.h"
#include "hal_clock.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "stm32wl55xx.h"
#include <stdint.h>

#define NUM_SEGMENTS 7U
#define NUM_DISPLAYS 2U

#define ONES_DISPLAY 0U
#define TENS_DISPLAY 1U

static uint8_t const segments[7U] = {DISP_QA, DISP_QB, DISP_QC, DISP_QD, DISP_QE, DISP_QF, DISP_QG};
static uint8_t const displays[2U] = {ONES_SEL, TENS_SEL};

static uint8_t const digit_table[10U][7U] = 
{
  {1, 1, 1, 1, 1, 1, 0}, /* 0 */
  {0, 1, 1, 0, 0, 0, 0}, /* 1 */
  {1, 1, 0, 1, 1, 0, 1}, /* 2 */
  {1, 1, 1, 1, 0, 0, 1}, /* 3 */
  {0, 1, 1, 0, 0, 1, 1}, /* 4 */
  {1, 0, 1, 1, 0, 1, 1}, /* 5 */
  {1, 0, 1, 1, 1, 1, 1}, /* 6 */
  {1, 1, 1, 0, 0, 0, 0}, /* 7 */
  {1, 1, 1, 1, 1, 1, 1}, /* 8 */
  {1, 1, 1, 1, 0, 1, 1}  /* 9 */
};

static volatile uint8_t current_segment = 0U;
static volatile uint8_t current_display = 0U;

/* ONES then TENS */
static uint8_t volatile display_data[NUM_DISPLAYS];

/**
 * @brief      Enable a segment
 *
 * @param[in]  s     segment to enable
 */
static void segment_enable(uint8_t s) {
  gpio_reset(segments[s]);
}

/**
 * @brief      Disable a segment
 *
 * @param[in]  s     segment to disable
 */
static void segment_disable(uint8_t s) {
  gpio_set(segments[s]);
}

/**
 * @brief      Enable a display
 *
 * @param[in]  s     display to enable
 */
static void display_enable(uint8_t d) {
  gpio_reset(displays[d]);
}

/**
 * @brief      Disable a display
 *
 * @param[in]  s     display to disable
 */
static void display_disable(uint8_t d) {
  gpio_set(displays[d]);
}

/**
 * @brief      Set the display data
 *
 * @param[in]  val  Value to display
 */
void seven_seg_display(uint8_t val) {
  uint8_t ones = 0U;
  uint8_t tens = 0U;

  if(val > 99U) {
    val = 99U;
  }
  
  tens = val / 10U;
  ones = val - (tens * 10U);

  display_data[ONES_DISPLAY] = ones;
  display_data[TENS_DISPLAY] = tens;
}

/**
 * @brief      Init the 7 segment hardware
 */
void seven_seg_init(void) {
	gpioa_clock_enable();
	gpioc_clock_enable();
	tim16_clock_enable();

  for(uint8_t i = 0; i < NUM_SEGMENTS; i++) {
    gpio_config_pin(segments[i], GPIO_MODE_OUTPUT);
    segment_disable(i);
  }

  for(uint8_t i = 0; i < NUM_DISPLAYS; i++) {
    gpio_config_pin(displays[i], GPIO_MODE_OUTPUT);
    display_disable(i);
  }

  timer_init(SEGMENT_TIMER, TIMER_CLK_INTERNAL, SEGMENT_TIMER_PRESCALER);
  timer_enable_isr(SEGMENT_TIMER, TIMER_ISR_UPDATE);
  timer_overflow(SEGMENT_TIMER, SEGMENT_TIMER_PERIOD);

  NVIC_EnableIRQ(SEGMENT_TIMER_IRQ);
  timer_start(SEGMENT_TIMER);
}

/**
 * @brief      Segment multiplexing interrupt handler
 */
void TIM16_IRQHandler(void) {
  timer_clear_uif_flag(SEGMENT_TIMER);
  uint8_t digit = display_data[current_display];

  if(current_segment < 7U) {
    if(digit_table[digit][current_segment] == 1U) {
      segment_enable(current_segment);
    }

    if(current_segment > 0U) {
      segment_disable(current_segment - 1U);
    }
  }

  current_segment++;

  /* the final segment needs a period to be lit */
  if(current_segment == NUM_SEGMENTS+1U) {
    current_segment = 0U;
    /* clear the current display */
    for(uint8_t i = 0U; i < NUM_SEGMENTS; i++) {
      segment_disable(segments[i]);
    }

    display_disable(current_display);
    current_display = (current_display + 1U) & 0x01U;
    display_enable(current_display);   
  }
}