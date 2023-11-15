#include <stdint.h>
#include "stm32wl55xx.h"
#include "port_spi.h"
#include "../hal_spi.h"

#define SPI_GET_XFER_METHOD(c) ((c) & 0x000FU)
#define SPI_GET_PRESCALER(c) ((c) & 0x00F0U)
#define SPI_GET_MODE(c) ((c) & 0x0F00U)

typedef struct spi_transfer spi_transfer;
struct spi_transfer {
	uint8_t *tx_ptr;
	uint8_t *rx_ptr;
	volatile uint16_t tx_count;
	volatile uint16_t rx_count;
};

static spi_transfer xfer_buffer[NUM_SPIS];

static SPI_TypeDef *spi_ports[NUM_SPIS] = {SPI1, SPI2, SUBGHZSPI};

void spi_init(uint8_t port, uint16_t config) {
	switch(SPI_GET_XFER_METHOD(config)) {
		case SPI_POLLING:
		break;

		case SPI_INTERRUPTS:
		break;

		case SPI_DMA:
		break;

		default:
			/* do nothing */
		break;
	}
}

/*
void spi_transfer(uint8_t port, uint8_t *tx_data, uint8_t *rx_data, uint16_t size) {
	xfer_buffer[port].tx_ptr = tx_data;
	xfer_buffer[port].rx_ptr = rx_data;
	xfer_buffer[port].tx_count = size;
	xfer_buffer[port].rx_count = size;



}
*/