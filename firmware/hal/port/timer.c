#include <stdint.h>
#include <stddef.h>
#include "stm32g441xx.h"
#include "port_timer.h"
#include "../hal_timer.h"

#define TIMER_GET_TIMER(t) (((t) & 0xF0U) >> 4U)
#define TIMER_GET_CHANNEL(t) ((t) & 0x0FU)
#define TIMER_GET_CLKSRC(t) ((t) & 0x000FU)
#define TIMER_GET_MODE(t) ((t) & 0x0F00U)
#define TIMER_GET_TRGO(t) ((t) & 0x0070U)
#define TIMER_GET_INPUT(t) ((t) & 0xF000U)

#define BOTH 0x0AU
#define FALLING 0x02U

static TIM_TypeDef *timer_ports[NUM_TIMERS] = {
	TIM1, TIM2, TIM3, TIM4, TIM6,
	TIM7, TIM8, TIM15, TIM16, TIM17
};

/** 
 * @brief      Set the SMS bits in the SMCR register
 *
 * @param      t     Timer peripheral pointer
 * @param[in]  src   The source
 */
static void timer_set_clock_source(uint8_t t, uint8_t src) {
	TIM_TypeDef *tim = NULL;

	switch(t << 4U) {
		case TIMER1:
		case TIMER2:
		case TIMER3:
		case TIMER4:
		case TIMER8:	
			tim = timer_ports[t];
			tim->SMCR |= (src << TIM_SMCR_SMS_Pos);

			if(src == TIMER_CLK_EXTERNAL) {
				/* set clock input to TI1FP1 */
				tim->SMCR |= (5U << TIM_SMCR_TS_Pos);
			}
		break;

		default:
			/* do nothing */
		break;
	}
}

/**
 * @brief      Get the index of the CCMRx register output bits for the channel
 *
 * @param[in]  ch    Channel number
 *
 * @return     CCMRx output mode bit index
 */
static uint8_t timer_ccmr_mode_index(uint8_t ch) {
	uint8_t index;
	switch(ch) {
		case 0U:
		case 2U:
			index = 4U;
		break;

		case 1U:
		case 4U:
			index = 12U;
		break;

		default:
			// TODO: handle this? 
			index = 0U;
		break;
	}
	return index;
}

static void timer_output_enable(uint8_t tim) {
	uint8_t t = TIMER_GET_TIMER(tim);
	timer_ports[t]->CCER |= (1U << (TIMER_GET_CHANNEL(tim) * 4U)); /* enable the output */
	
	if((t == TIMER1) || (t == TIMER8)) {
		timer_ports[t]->BDTR |= TIM_BDTR_MOE; /* master out enable */
	}
}

static void timer_ccmr_init(uint8_t timer, uint8_t channel, uint8_t mode) {
	if(channel < 2U) { 
		/* channels 1 and 2 share CCMR1 */
		timer_ports[timer]->CCMR1 |= (mode << timer_ccmr_mode_index(channel));
	} else {
		/* channels 3 and 4 share CCMR2 */
		timer_ports[timer]->CCMR2 |= (mode << timer_ccmr_mode_index(channel));
	}	
}

/**
 * @brief      Basic timer initialisation
 *
 * @param[in]  tim        The timer to init
 * @param[in]  config     Timer config
 * @param[in]  prescaler  The clock prescaler
 */
void timer_init(uint8_t tim, uint16_t config, uint16_t prescaler) {
	uint8_t t = TIMER_GET_TIMER(tim);
	uint8_t channel = TIMER_GET_CHANNEL(tim);

	/* setup timer clock */
	timer_set_clock_source(t, TIMER_GET_CLKSRC(config));

	if(prescaler > 0U) {
		timer_ports[t]->PSC = prescaler-1U;
	}
	
	/* set trgo */
	timer_ports[t]->CR2 |= TIMER_GET_TRGO(config);

	/* set up timer polarity */
	switch(TIMER_GET_INPUT(config)) {
		case TIMER_INPUT_FALLING:
			timer_ports[t]->CCER |= (FALLING << (channel * 4U));
		break;

		case TIMER_INPUT_BOTH:
			timer_ports[t]->CCER |= (BOTH << (channel * 4U));
		break;

		case TIMER_INPUT_RISING:
		default:
			/* do nothing, default to rising edge */
		break;
	}

	/* set up timer mode */
	switch(TIMER_GET_MODE(config)) {
		case TIMER_MODE_PWM:
			timer_ccmr_init(t, channel, 0x06U);
			timer_output_enable(tim);
		break;

		case TIMER_MODE_OC:
			timer_ccmr_init(t, channel, 0x01U);
			timer_output_enable(tim);
		break;

		case TIMER_MODE_OC_TOGGLE:
			timer_ccmr_init(t, channel, 0x03U);
			timer_output_enable(tim);
		break;

		case TIMER_MODE_IC:

		break;
	}
}

void timer_overflow(uint8_t tim, uint16_t ovf) {
	uint8_t t = TIMER_GET_TIMER(tim);
	// TODO check t valid
	timer_ports[t]->ARR = ovf;
}

void timer_pwm_duty(uint8_t tim, uint16_t duty) {
	uint8_t t = TIMER_GET_TIMER(tim);
	uint8_t ch = TIMER_GET_CHANNEL(tim);
	// TODO check t valid
	volatile uint32_t *ccr = (&timer_ports[t]->CCR1) + (ch * 4U);
	*ccr = duty;
}

void timer_compare_value(uint8_t tim, uint16_t val) {
	uint8_t t = TIMER_GET_TIMER(tim);
	uint8_t ch = TIMER_GET_CHANNEL(tim);
	// TODO check t valid
	volatile uint32_t *ccr = (&timer_ports[t]->CCR1) + (ch * 4U);
	*ccr = val;
}

void timer_enable_isr(uint8_t tim, uint8_t isr) {
	uint8_t t = TIMER_GET_TIMER(tim);
	uint8_t ch = TIMER_GET_CHANNEL(tim);

	switch(isr) {
		case TIMER_ISR_UPDATE:
			timer_ports[t]->DIER |= TIM_DIER_UIE;
		break;

		case TIMER_ISR_CCMP:
			timer_ports[t]->DIER |= (1U << (TIM_DIER_CC1IE_Pos + ch));
		break;

		default:
			/* do nothing */
		break;
	}

}

void timer_clear_uif_flag(uint8_t tim) {
	uint8_t t = TIMER_GET_TIMER(tim);
	timer_ports[t]->SR &= ~TIM_SR_UIF;
}

/**
 * @brief      Start timer
 *
 * @param[in]  tim   The timer
 */
void timer_start(uint8_t tim) {
	timer_ports[TIMER_GET_TIMER(tim)]->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief      Stop timer
 *
 * @param[in]  tim   The timer
 */
void timer_stop(uint8_t tim) {
	timer_ports[TIMER_GET_TIMER(tim)]->CR1 |= ~TIM_CR1_CEN;
}