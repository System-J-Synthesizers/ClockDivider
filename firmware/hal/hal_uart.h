#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>
#include "./port/port_uart.h"

/* internal ring buffer sizes (MUST BE POWER OF 2) */
#define UART_TXBUF_SIZE 256U
#define UART_RXBUF_SIZE 256U

/* state of the internal ring buffers */
#define UART_OK 0U
#define UART_EMPTY 1U
#define UART_FULL 2U

void uart_init(uint8_t u);
uint8_t uart_tx(uint8_t u, uint8_t *buf, uint16_t count);
uint8_t uart_rx(uint8_t u, uint8_t *buf, uint16_t count);

#endif /* HAL_UART_H */