#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

void adc_init(uint16_t ch);
uint16_t adc_read(uint16_t ch);

#endif /* HAL_ADC_H */
