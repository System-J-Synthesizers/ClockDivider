#include <stdint.h>
#include <stddef.h>
#include "stm32wl55xx.h"
#include "port_rng.h"
#include "../hal_rng.h"

static uint32_t rng_buf[RNG_BUFSIZE];
static uint8_t rng_read_index = 0U;
static volatile uint8_t rng_write_index = 0U;
static volatile uint8_t rng_count = 0U;

/**
 * @brief      RNG interrupt handler
 */
void RNG_IRQHandler(void) {
	/* simple ring buffer, adds samples until it is full */
  if((RNG->SR & RNG_SR_DRDY) == RNG_SR_DRDY) {
    rng_buf[rng_write_index] = RNG->DR;
    rng_write_index = (rng_write_index + 1U) & (RNG_BUFSIZE-1U);
    rng_count++;

    /* if the buffer is full stop generating interrupts */
    if(rng_count == RNG_BUFSIZE) {
      RNG->CR &= ~RNG_CR_IE; 
    }    
  }
}

/**
 * @brief      Sets up the random number generator
 */
void rng_init(void) {
  /* set RNG clock source to MSI */
  RCC->CCIPR |= (3U << RCC_CCIPR_RNGSEL_Pos);   

  /* set up the random sources, see the reference manual */
  RNG->CR &= ~RNG_CR_RNGEN;
  RNG->CR |= RNG_CR_CONDRST;

  RNG->HTCR = 0x17590ABCUL; /* reference manual 22.7.4 */ 
  RNG->HTCR = 0x0000AA74UL; /* reference manual 22.6.2 */

  NVIC_EnableIRQ(RNG_IRQn);

  RNG->CR &= ~RNG_CR_CONDRST;
  RNG->CR |= (RNG_CR_RNGEN | RNG_CR_IE);
}

/**
 * @brief      Reads samples from the internal ring buffer. 
 * Enables interrupts if buffer threshold reached.
 *
 * @return     zero if no random samples available otherwise random data.
 */
uint32_t rng_read(void) {
  uint32_t result = 0UL;

  if(rng_count > 0U) {
    result = rng_buf[rng_read_index];
    rng_read_index = (rng_read_index + 1U) & (RNG_BUFSIZE-1U);
    rng_count--;

    if(rng_count < RNG_FIFO_THRESHOLD) {
      RNG->CR |= RNG_CR_IE;
    }  
  }
  /* returns zero if rng empty, up to caller what to do here */
  return result;
}
