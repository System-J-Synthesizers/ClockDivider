#include <stdint.h>
#include "stm32wl55xx.h"
#include "port_dma.h"
#include "../hal_dma.h"
#include "../hal_assert.h"

#define DMA_GET_CONTROLLER(d) (((d) & 0xF000U) >> 12U)
#define DMA_GET_CHANNEL(d) (((d) & 0x0F00U) >> 8U)
#define DMA_GET_REQ_NUM(d) ((d) & 0x00FFU)

static DMA_TypeDef *dma_controllers[DMA_NUM] = {DMA1, DMA2};

static DMA_Channel_TypeDef *dma_channels[DMA_NUM][DMA_CHANNEL_NUM] = {
	{DMA1_Channel1, DMA1_Channel2, DMA1_Channel3, DMA1_Channel4, DMA1_Channel5, DMA1_Channel6, DMA1_Channel7},
	{DMA2_Channel1, DMA2_Channel2, DMA2_Channel3, DMA2_Channel4, DMA2_Channel5, DMA2_Channel6, DMA1_Channel7}
};

static DMAMUX_Channel_TypeDef *dmamux_channels[DMA_NUM * DMA_CHANNEL_NUM] = {
	DMAMUX1_Channel0, DMAMUX1_Channel1, DMAMUX1_Channel2, DMAMUX1_Channel3, DMAMUX1_Channel4, DMAMUX1_Channel5, DMAMUX1_Channel6,
	DMAMUX1_Channel7, DMAMUX1_Channel8, DMAMUX1_Channel9, DMAMUX1_Channel10, DMAMUX1_Channel11, DMAMUX1_Channel12, DMAMUX1_Channel13
};

void dma_init(uint16_t channel, uint32_t config) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	uint8_t req_num = DMA_GET_REQ_NUM(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CCR = config;

	if(ch_ctl > DMA_1) {
		dmamux_channels[ch_num+7U]->CCR |= (req_num << DMAMUX_CxCR_DMAREQ_ID_Pos);
	} else {
		dmamux_channels[ch_num]->CCR |= (req_num << DMAMUX_CxCR_DMAREQ_ID_Pos);
	}
}

void dma_set_src_addr(uint16_t channel, uint32_t address) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CMAR = address;
}

void dma_set_dst_addr(uint16_t channel, uint32_t address) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CPAR = address;
}

void dma_set_xfer_count(uint16_t channel, uint16_t count) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CNDTR = count;
}

void dma_enable_isr(uint16_t channel, uint8_t isr) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CCR |= isr;
}

void dma_disable_isr(uint16_t channel, uint8_t isr) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CCR &= ~isr;	
}

void dma_enable(uint16_t channel) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CCR |= DMA_CCR_EN;
}

void dma_disable(uint16_t channel) {
	uint8_t ch_ctl = DMA_GET_CONTROLLER(channel);
	uint8_t ch_num = DMA_GET_CHANNEL(channel);
	DMA_Channel_TypeDef *d = dma_channels[ch_ctl][ch_num];
	d->CCR &= ~DMA_CCR_EN;
}