#include <stdint.h>
#include "stm32wl55xx.h"
#include "port_chip.h"
#include "../hal_chip.h"
#include "../hal_assert.h"

void chip_enable_isrs(void) {
	asm("cpsie i\n\t");
}

void chip_disable_isrs(void) {
	asm("cpsid i\n\t");
}


