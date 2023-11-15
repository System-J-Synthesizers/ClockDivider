#include "hal_chip.h"
#include "hal_clock.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "seven_segment.h"
#include "channels.h"
#include "state_machine.h"
#include <cross_studio_io.h>

void assert_failed(void) {
  debug_printf("!!! HAL ASSERT FAILED !!!\n");
  while(1U) {
  }
}

void main(void) {
  seven_seg_init();
  channel_leds_init();
  /* channel_init(1-4) */
  state_machine_init();

  while(1) {
  	/* wait for interrupt */
  }
}