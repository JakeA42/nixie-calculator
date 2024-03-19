/*
 * DMA controls
 * Based on code from Luca Davidian
 * http://www.lucadavidian.com/2018/03/08/wifi-controlled-neo-pixels-strips/
 */



#include "samd21.h"
#include <string.h>
#include "DMA.h" 

#define MAX_DESCRIPTORS 2

__attribute__((__aligned__(16))) static DmacDescriptor descriptor_section[MAX_DESCRIPTORS];
__attribute__((__aligned__(16))) static DmacDescriptor write_back_section[MAX_DESCRIPTORS];
static int used_channels = 0;

int volatile dma_frame = 0;

void DMA_init(void)
{
	static int initialized = 0;
	
	if (!initialized)
	{
		/* enable peripheral clock */
		PM->APBBMASK.bit.DMAC_ = 1;
		
		/* enable AHB master clock */
		PM->AHBMASK.bit.DMAC_ = 1;

		/* configure DMA controller */
		DMAC->BASEADDR.reg = (uint32_t)descriptor_section;                    // descriptor memory section start
		DMAC->WRBADDR.reg = (uint32_t)write_back_section;                     // descriptor write-back memory section start
		DMAC->CTRL.bit.LVLEN0 = 1;                                            // enable level 0 priority
		DMAC->CTRL.bit.LVLEN1 = 1;                                            // enable level 1 priority
		DMAC->CTRL.bit.LVLEN2 = 1;                                            // enable level 2 priority
		DMAC->CTRL.bit.LVLEN3 = 1;                                            // enable level 3 priority
		
		/* DMA is initialized */
		initialized = 1;
	}
}

/* add the first least significant free channel with its descriptor (TODO: implement linked descriptors) */
void DMA_add_channel(int source, DmacDescriptor *descriptor)
{
	/* disable DMA if enabled */
	if (DMAC->CTRL.bit.DMAENABLE)
		DMAC->CTRL.bit.DMAENABLE = 0;
	while (DMAC->CTRL.bit.DMAENABLE)
		;
		
	/* add transfer descriptor to transfer descriptor section (before enabling channel!) */
	memcpy(descriptor_section + used_channels * sizeof(DmacDescriptor), descriptor, sizeof(DmacDescriptor));
		
	/* configure and enable first least significant free channel */
	DMAC->CHID.bit.ID = used_channels++;                                  // use first free channel 
	DMAC->CHCTRLB.bit.LVL = 0x00;                                         // channel priority level 0
	DMAC->CHCTRLB.bit.TRIGACT = 0x02;                                     // one trigger each beat transfer
	DMAC->CHCTRLB.bit.TRIGSRC = source;                                   // select trigger source
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;                             // enable channel 
		
	/* enable DMA block transfer complete interrupt */
	DMAC->CHINTENSET.bit.TCMPL = 1;                                       // enable DMA block transfer complete interrupt
	NVIC_EnableIRQ(DMAC_IRQn);                                            // enable DMA interrupt in NVIC
}

void DMA_enable(void)
{
	/* enable DMA controller */
	DMAC->CTRL.bit.DMAENABLE = 1;
}

void DMAC_Handler(void)
{
	//static int count;
	for (int channel = 0 ; channel < used_channels ; channel++)  // check interrupt for every registered channel
	{
		DMAC->CHID.bit.ID = channel;      
		if (DMAC->CHINTFLAG.bit.TCMPL && DMAC->CHINTENSET.bit.TCMPL)
		{
			if (DMAC->CHID.bit.ID == 0x00)   // handle SPI transfer complete interrupt
			{
				DMAC->CHINTFLAG.bit.TCMPL = 1;                 // acknowledge interrupt
				DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;      // re-enable DMA channel
			}
		}
	}
}

