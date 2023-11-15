#include <debugio.h>
#include "stm32wlxx.h"

#include "hal_clock.h"
#include "hal_gpio.h"
#include "hal_timer.h"

void assert_failed(void) {
  debug_printf("!!! HAL ASSERT FAILED !!!\n");
  while(1U) {
  }
}

#define LED1  (PORT_B | 15U)
#define LED2  (PORT_B | 9U)
#define LED3  (PORT_B | 11U)

//#define B1  (PORT_A | 0U)
//#define B2  (PORT_A | 1U)
#define TENS_SEL  (PORT_A | 11U)
#define ONES_SEL  (PORT_A | 12U)

#define QA  (PORT_C | 2U)
#define QB  (PORT_C | 1U)
#define QC  (PORT_A | 7U)
#define QD  (PORT_A | 4U)
#define QE  (PORT_A | 9U)
#define QF  (PORT_A | 5U)
#define QG  (PORT_A | 6U)

#define DIGIT_TIM TIMER16
#define BUTTON_TIM TIMER17

volatile uint8_t count = 0;

uint8_t button_state = 0U;
uint8_t last_button_state = 1U;

#define NUM_BUTTONS 2U

#define TEST_PIN (PORT_B | 10U)
#define TEST_PIN2 (PORT_B | 8U)

//uint8_t buttons[NUM_BUTTONS] = {B1, B2};
//uint8_t leds[NUM_BUTTONS] = {LED1, LED2};
//uint8_t button_states[NUM_BUTTONS] = {0U, 0U};
//uint8_t last_button_states[NUM_BUTTONS] = {0U, 0U};

//void TIM16_IRQHandler(void) {
//  timer_clear_uif_flag(INPUT_TIM);

//  for(uint8_t i = 0; i < NUM_BUTTONS; i++) {
//    button_states[i] = gpio_get(buttons[i]);
//    if((button_states[i] == 0U) && (last_button_states[i] == 1U)) {
//      gpio_toggle(leds[i]);
//    }
//    last_button_states[i] = button_states[i];
//  }
//} 

static uint8_t digit_table[10U][7U] = 
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

uint8_t segments[7U] = {QA, QB, QC, QD, QE, QF, QG};
uint8_t displays[2U] = {ONES_SEL, TENS_SEL};

uint8_t current_segment = 0U;
uint8_t current_display = 0U;

/* ONES then TENS */
uint8_t display_data[2U] = {1, 4};

uint8_t test;

void TIM16_IRQHandler(void) {
  gpio_set(TEST_PIN);
  timer_clear_uif_flag(DIGIT_TIM);

  uint8_t digit = display_data[current_display];

  if(current_segment < 7U) {

    for(uint8_t i = 0; i < 7U; i++) {
      gpio_set(segments[i]);
      if(i == current_segment) {
        if(digit_table[digit][i] == 1U) {
          gpio_reset(segments[i]);
        }    
      }
    }

  }

  current_segment++;
  
  if(current_segment == 8U) {
    current_segment = 0U;

    for(uint8_t i = 0U; i < 7U; i++) {
      gpio_set(segments[i]);
    }

    gpio_set(displays[current_display]);
    current_display = (current_display + 1U) & 0x01U;
    gpio_reset(displays[current_display]);
  }

  gpio_reset(TEST_PIN);
}

void TIM17_IRQHandler(void) {
  timer_clear_uif_flag(BUTTON_TIM);
  gpio_set(TEST_PIN2);
  gpio_reset(TEST_PIN2);
}

void main(void) {
  gpioa_clock_enable();
  gpiob_clock_enable();
  gpioc_clock_enable();

  tim16_clock_enable();
  tim17_clock_enable();

  gpio_config_pin(LED1, GPIO_MODE_OUTPUT);
  gpio_config_pin(LED2, GPIO_MODE_OUTPUT);
  gpio_config_pin(LED3, GPIO_MODE_OUTPUT);

  gpio_config_pin(TEST_PIN, GPIO_MODE_OUTPUT);
  gpio_config_pin(TEST_PIN2, GPIO_MODE_OUTPUT);

 // gpio_config_pin(B1, (GPIO_MODE_INPUT | GPIO_PULL_UP));
 //gpio_config_pin(B2, (GPIO_MODE_INPUT | GPIO_PULL_UP));
  gpio_config_pin(TENS_SEL, GPIO_MODE_OUTPUT);
  gpio_set(TENS_SEL);

  gpio_config_pin(ONES_SEL, GPIO_MODE_OUTPUT);
  gpio_set(ONES_SEL);


  
  for(uint8_t i = 0; i < 7U; i++) {
    gpio_config_pin(segments[i], GPIO_MODE_OUTPUT);
    gpio_set(segments[i]);
  }

  //gpio_reset(segments[0]);

  timer_init(DIGIT_TIM, TIMER_CLK_INTERNAL, 4U);
  timer_enable_isr(DIGIT_TIM, TIMER_ISR_UPDATE);
  timer_overflow(DIGIT_TIM, 1500U);

  timer_init(BUTTON_TIM, TIMER_CLK_INTERNAL, 4U);
  timer_enable_isr(BUTTON_TIM, TIMER_ISR_UPDATE);
  timer_overflow(BUTTON_TIM, 50000U);

  NVIC_EnableIRQ(TIM16_IRQn);
  NVIC_EnableIRQ(TIM17_IRQn);
  
  timer_start(DIGIT_TIM);
  timer_start(BUTTON_TIM);

  while(1) {
    //gpio_reset(TENS_SEL);
    //for(uint8_t i = 0; i < 7U; i++) {
    //  if(i > 0U) {
    //    gpio_set(segments[i-1]);
    //    gpio_reset(segments[i]);
    //  } else {
    //    gpio_reset(segments[i]);
    //  }
    //}
    //gpio_set(segments[6U]);
    //gpio_set(TENS_SEL);

    //gpio_reset(ONES_SEL);
    //for(uint8_t i = 0; i < 7U; i++) {
    //  if(i > 0U) {
    //    gpio_set(segments[i-1]);
    //    gpio_reset(segments[i]);
    //  } else {
    //    gpio_reset(segments[i]);
    //  }
    //}
    //gpio_set(segments[6U]);
    //gpio_set(ONES_SEL);

  }
}