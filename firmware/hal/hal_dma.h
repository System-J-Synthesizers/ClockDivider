#ifndef HAL_DMA_H
#define HAL_DMA_H

/**
 * Generic interface to DMA driver.
 * Underlying chip implementation is defined under /chips and will be under /port in an application
 * 
 * Channels are configured by OR-ing the settings below into a configuration word which is handed to the driver
 * 
 * The channel information is contained under port_dma.h and depends on the chip.
 * For example some STM32s have two DMA controllers and a DMAMUX to set the request ID, so the channel id would be
 * defined as follows:
 * 	#define DMA_CH1 (DMA1 | DMA1_Channel1 | DMA_REQ_n)
 * where DMA1 is the underlying implementation defined in port_dma.h 
 */

#include <stdint.h>
#include "./port/port_dma.h"

/* Channel setting positions are contained in the port header */

/**
 * M2P - Memory to peripheral
 * P2M - Peripheral to memory
 */
#define DMA_DIR_P2M (0U << DMA_DIR_POS)
#define DMA_DIR_M2P (1U << DMA_DIR_POS)

/**
 * circular mode will increment and wrap the memory pointer around at the end of the transfer automatically
 */
#define DMA_MODE_NON_CIRC (0x0U << DMA_MODE_POS)
#define DMA_MODE_CIRC 		(0xAU << DMA_MODE_POS)

/**
 * Source and destination are both set to this size
 */
#define DMA_SIZE_BYTE (0x00U << DMA_SIZE_POS)
#define DMA_SIZE_HALF (0x05U << DMA_SIZE_POS)
#define DMA_SIZE_WORD (0x0AU << DMA_SIZE_POS)
/* Channel settings */

/* Interrupts 
 * (some dma peripherals have multiple interrups per channel and are defined in the chips port_dma.h 
 */
#define DMA_XFER_COMPLETE (1U << DMA_XFER_CPLT_POS)
#define DMA_XFER_ERROR (1U << DMA_XFER_ERROR_POS)

/* these functions are chip dependant */
void dma_init(uint16_t channel, uint32_t config);
void dma_set_src_addr(uint16_t channel, uint32_t address);
void dma_set_dst_addr(uint16_t channel, uint32_t address);
void dma_set_xfer_count(uint16_t channel, uint16_t count);
void dma_enable_isr(uint16_t channel, uint8_t isr);
void dma_disable_isr(uint16_t channel, uint8_t isr);
void dma_enable(uint16_t channel);
void dma_disable(uint16_t channel);

#endif /* HAL_DMA_H */
