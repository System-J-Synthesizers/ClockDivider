#include <stdint.h>
#include "stm32wl55xx.h"
#include "port_dac.h"
#include "../hal_dac.h"

#define DAC_GET_MODE(d) ((d) & 0x00F0U)
#define DAC_GET_TRIGGER(d) ((d) & 0x000FU)
#define DAC_GET_BUFFER(d) ((d) & 0x0F00U)

void dac_init(uint8_t ch, uint16_t config) {
	(void)ch; /* this particular chip only has one DAC channel */
	switch(DAC_GET_MODE(config)) {
		case DAC_MODE_SINGLE:
			DAC->CR |= DAC_CR_TEN1;
			DAC->CR |= DAC_CR_EN1;
		break;

		case DAC_MODE_STREAM:
			/* enable DMA and set up trigger */
			DAC->CR |= (DAC_CR_DMAEN1 | DAC_CR_TEN1 | DAC_GET_TRIGGER(config));
		break;

		default:
			/* TODO: nothing here? */
		break;
	}
}

/**
 * @brief      Set the dac to value in data
 *
 * @param[in]  ch    DAC channel
 * @param[in]  data  12 bit data
 */
void dac_set(uint8_t ch, uint16_t data) {
	(void)ch; /* this particular chip only has one DAC channel */
	DAC->DHR12R1 = data;
	DAC->SWTRIGR |= 1U;
}

/**
 * @brief      Start the DAC stream. Assumed DMA and Timers setup already
 *
 * @param[in]  ch    DAC channel
 */
void dac_start_stream(uint8_t ch) {
	(void)ch; /* this particular chip only has one DAC channel */
	DAC->CR |= DAC_CR_EN1;
}