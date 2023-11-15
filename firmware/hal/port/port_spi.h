#ifndef PORT_SPI_H
#define PORT_SPI_H

#include <stdint.h>

#define SPI_DMA 0x0002U

#define SPI_CLK_DIV2 		0x0000U
#define SPI_CLK_DIV4  	0x0010U
#define SPI_CLK_DIV8 		0x0020U
#define SPI_CLK_DIV16 	0x0030U
#define SPI_CLK_DIV32 	0x0040U
#define SPI_CLK_DIV64		0x0050U
#define SPI_CLK_DIV128	0x0060U
#define SPI_CLK_DIV256	0x0070U

#define SPI_MODE0 	0x0000U
#define SPI_MODE1   0x0100U
#define SPI_MODE2  	0x0200U
#define SPI_MODE3 	0x0300U

#define NUM_SPIS 	3U

#define SPI_1 			0x00U
#define SPI_2 			0x01U
#define SPI_SUBGHZ 	0x03U

#endif /* PORT_SPI_H */
