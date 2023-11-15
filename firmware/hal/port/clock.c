#include "stm32wl55xx.h"

void dma1_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
}

void dma2_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

void dmamux1_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;
}

void crc_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
}

void gpioa_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
}

void gpiob_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
}

void gpioc_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}

void gpioh_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
}

void pka_clock_enable(void) {
    RCC->AHB3ENR |= RCC_AHB3ENR_PKAEN;
}

void aes_clock_enable(void) {
    RCC->AHB3ENR |= RCC_AHB3ENR_AESEN;
}

void rng_clock_enable(void) {
    RCC->AHB3ENR |= RCC_AHB3ENR_RNGEN;
}

void hsem_clock_enable(void) {
    RCC->AHB3ENR |= RCC_AHB3ENR_HSEMEN;
}

void ipcc_clock_enable(void) {
    RCC->AHB3ENR |= RCC_AHB3ENR_IPCCEN;
}

void flash_clock_enable(void) {
    RCC->AHB3ENR |= RCC_AHB3ENR_FLASHEN;
}

void tim2_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
}

void rtcapb_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_RTCAPBEN;
}

void wwdg_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN;
}

void spi2_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
}

void usart2_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
}

void i2c1_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
}

void i2c2_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;
}

void i2c3_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
}

void dac_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_DACEN;
}

void lptim1_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_LPTIM1EN;
}

void lpuart1_clock_enable(void) {
    RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
}

void lptim2_clock_enable(void) {
    RCC->APB1ENR2 |= RCC_APB1ENR2_LPTIM2EN;
}

void lptim3_clock_enable(void) {
    RCC->APB1ENR2 |= RCC_APB1ENR2_LPTIM3EN;
}

void adc_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
}

void tim1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
}

void spi1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}

void usart1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

void tim16_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
}

void tim17_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
}

void subghzspi_clock_enable(void) {
    RCC->APB3ENR |= RCC_APB3ENR_SUBGHZSPIEN;
}

