#ifndef PORT_DMA_H
#define PORT_DMA_H

/* based on the register definition */
#define DMA_DIR_POS 4U
#define DMA_MODE_POS 4U
#define DMA_SIZE_POS 8U
#define DMA_XFER_CPLT_POS 1U
#define DMA_XFER_ERROR_POS 3U

/* DMA implementation */
#define DMA_NUM 2U
#define DMA_CHANNEL_NUM 7U

/* Chip has two DMA controllers */
#define DMA_1 0U
#define DMA_2 1U

/* DMA request IDs (RM0453 page 482) */
#define DMA_REQ_ADC 5U
#define DMA_REQ_DAC_OUT1 6U
#define DMA_REQ_SPI1_RX 7U
#define DMA_REQ_SPI1_TX 8U
#define DMA_REQ_SPI2_RX 9U
#define DMA_REQ_SPI2_TX 10U
#define DMA_REQ_I2C1_RX 11U
#define DMA_REQ_I2C1_TX 12U
#define DMA_REQ_I2C2_RX 13U
#define DMA_REQ_I2C2_TX 14U
#define DMA_REQ_I2C3_RX 15U
#define DMA_REQ_I2C3_TX 16U
#define DMA_REQ_USART1_RX 17U
#define DMA_REQ_USART1_TX 18U
#define DMA_REQ_USART2_RX 19U
#define DMA_REQ_USART2_TX 20U
#define DMA_REQ_LPUART1_TX 22U
#define DMA_REQ_TIM1_CH1 23U
#define DMA_REQ_TIM1_CH2 24U
#define DMA_REQ_TIM1_CH3 25U
#define DMA_REQ_TIM1_CH4 26U
#define DMA_REQ_TIM1_UP 27U
#define DMA_REQ_TIM1_TRIG 28U
#define DMA_REQ_TIM1_COM 29U
#define DMA_REQ_TIM2_CH1 30U
#define DMA_REQ_TIM2_CH2 31U
#define DMA_REQ_TIM2_CH3 32U
#define DMA_REQ_TIM2_CH4 33U
#define DMA_REQ_TIM2_UP 34U
#define DMA_REQ_TIM16_CH1 35U
#define DMA_REQ_TIM16_UP 36U
#define DMA_REQ_TIM17_CH1 37U
#define DMA_REQ_TIM17_UP 38U
#define DMA_REQ_AES_IN 39U
#define DMA_REQ_AES_OUT 40U
#define DMA_REQ_SUBGHZSPI_RX 41U
#define DMA_REQ_SUBGHZSPI_TX 42U

#endif /* PORT_DMA_H */
