#include "stm32g441xx.h"

void dma1_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
}

void dma2_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

void dmamux1_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;
}

void cordic_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_CORDICEN;
}

void fmac_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_FMACEN;
}

void flash_clock_enable(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_FLASHEN;
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

void gpiof_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
}

void gpiog_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOGEN;
}

void adc12_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;
}

void dac1_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_DAC1EN;
}

void dac3_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_DAC3EN;
}

void aes_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_AESEN;
}

void rng_clock_enable(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
}

void tim2_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
}

void tim3_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
}

void tim4_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
}

void tim6_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;
}

void tim7_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
}

void rtc_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_RTCAPBEN;
}

void wwdg_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN;
}

void spi2_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
}

void spi3_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
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

void usb_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_USBEN;
}

void fdcan_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN;
}

void pwr_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
}

void i2c3_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
}

void lptim1_clock_enable(void) {
    RCC->APB1ENR1 |= RCC_APB1ENR1_LPTIM1EN;
}

void lpuart1_clock_enable(void) {
    RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;
}

void ucpd1_clock_enable(void) {
    RCC->APB1ENR2 |= RCC_APB1ENR2_UCPD1EN;
}

void syscfg_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void tim1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
}

void spi1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}

void tim8_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
}

void usart1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

void tim15_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
}

void tim16_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
}

void tim17_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
}

void sai1_clock_enable(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;
}

