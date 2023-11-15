#ifndef HAL_DAC_H
#define HAL_DAC_H

#include <stdint.h>
#include "./port/port_dac.h"

#define DAC_MODE_SINGLE 0x0000U
#define DAC_MODE_STREAM 0x0010U

void dac_init(uint8_t ch, uint16_t config);
void dac_set(uint8_t ch, uint16_t data);
void dac_start_stream(uint8_t ch);

#endif /* HAL_DAC_H */
