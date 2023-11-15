#ifndef HAL_CHIP_H
#define HAL_CHIP_H

#include "./port/port_chip.h"

void chip_enable_isrs(void);
void chip_disable_isrs(void);

void chip_init(void);

#endif /* HAL_CHIP_H */
