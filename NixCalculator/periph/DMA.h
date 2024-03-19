/*
 * DMA controls
 * Based on code from Luca Davidian
 * http://www.lucadavidian.com/2018/03/08/wifi-controlled-neo-pixels-strips/
 */

#ifndef DMA_H_
#define DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_SOURCE_SERCOM0  0x02
#define DMA_SOURCE_SERCOM2  0x06 // Refer to datasheet section 20.8.19
#define DMA_SOURCE_ADC      0x27 

extern volatile int dma_frame;

void DMA_init(void);
void DMA_add_channel(int source, DmacDescriptor *descriptor);
void DMA_enable(void);

#ifdef __cplusplus
};
#endif

#endif /* DMA_H_ */