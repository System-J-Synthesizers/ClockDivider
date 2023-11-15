#ifndef PORT_DAC_H
#define PORT_DAC_H

#include <stdint.h>
#include "stm32wl55xx.h"

#define DAC_1 0U

#define DAC_TRIGGER_SW 			0U
#define DAC_TRIGGER_TIM1 		1U 	/* trigger 1 */
#define DAC_TRIGGER_TIM2 		2U 	/* trigger 2 */
#define DAC_TRIGGER_LPTIM1	11U /* trigger 11 */
#define DAC_TRIGGER_LPTIM2	12U /* trigger 12 */
#define DAC_TRIGGER_LPTIM3	13U /* trigger 13 */
#define DAC_TRIGGER_EXTI9		14U /* trigger 14 */

#define DAC_BUFFER_DISABLED 0x0000U
#define DAC_BUFFER_ENABLED 	0x0100U

#endif /* PORT_DAC_H */
