//#define CLOCK_IN (PORT_A | 5U)
//#define CLOCK_OUT (PORT_B | 3U)

//uint8_t arr = 1U;

//void main(void) {
//  gpioa_clock_enable();
//  gpiob_clock_enable();
//  tim2_clock_enable();

//  /* enable external clock */
//  gpio_config_pin(CLOCK_IN, GPIO_MODE_AF1);
//  TIM2->SMCR |= ((5U << TIM_SMCR_TS_Pos) | (7U << TIM_SMCR_SMS_Pos));
//  TIM2->CCER |= (TIM_CCER_CC1P | TIM_CCER_CC1NP);

//  /* set up channel 2*/
//  gpio_config_pin(CLOCK_OUT, GPIO_MODE_AF1);
  
//  /* standard output compare config */
//  timer_init(TIMER2_CH2, TIMER_MODE_OC_TOGGLE, 0U);
//  timer_overflow(TIMER2_CH2, 1U);
//  timer_compare_value(TIMER2_CH2, 0U);

//  timer_start(TIMER2_CH2);


//  while(1) {
//    TIM2->ARR = arr;
//  }
//}