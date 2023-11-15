#include "hal_chip.h"
#include "hal_clock.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "seven_segment.h"
#include <cross_studio_io.h>

void assert_failed(void) {
  debug_printf("!!! HAL ASSERT FAILED !!!\n");
  while(1U) {
  }
}

void main(void) {
  seven_seg_init();

  while(1) {
  	/* wait for interrupt */
  }
}