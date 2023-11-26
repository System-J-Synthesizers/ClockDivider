#ifndef HAL_CHIP_H
#define HAL_CHIP_H

#include <stdint.h>
#include "./port/port_chip.h"

void chip_enable_isrs(void);
void chip_disable_isrs(void);
uint32_t chip_get_isr_state(void);

void chip_init(void);

#endif /* HAL_CHIP_H */
