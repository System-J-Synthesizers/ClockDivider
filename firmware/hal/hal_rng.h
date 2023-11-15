#ifndef HAL_RNG_H
#define HAL_RNG_H

#include <stdint.h>
#include "./port/port_rng.h"

void rng_init(void);
uint32_t rng_read(void);

#endif /* HAL_RNG_H */
