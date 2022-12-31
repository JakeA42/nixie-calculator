/*
 * SPI controls
 * 
 */

#include "samd21.h"
#include "SPI.h"
#include "pins.h"

/*
 * initialize SPI on sercom2 with MOSI on pad 0 (pin PA12)
 * Based on code from Luca Davidian
 * http://www.lucadavidian.com/2018/03/08/wifi-controlled-neo-pixels-strips/
 */
void ws2812_SPI_init()
{
	/* enable peripheral clock */
	PM->APBCMASK.bit.SERCOM2_ = 1;
	
	/* enable generic clock */
	GCLK->CLKCTRL.bit.ID = GCLK_CLKCTRL_ID_SERCOM2_CORE_Val;     // configure generic clock for SERCOM2
	
	GCLK->CLKCTRL.bit.GEN = GCLK_CLKCTRL_GEN_GCLK0_Val;          // source is generic clock generator 0 
	GCLK->CLKCTRL.bit.CLKEN = 1;                                 // enable generic clock
	
	/* select GPIO pins alternative function */
	PORT->Group[BACKLIGHT_MOSI_PORT].PINCFG[BACKLIGHT_MOSI_PIN].bit.PMUXEN = 1;                // enable alternative function for pin
	PORT->Group[BACKLIGHT_MOSI_PORT].PMUX[BACKLIGHT_MOSI_PIN/2].bit.PMUXE = BACKLIGHT_MOSI_PMUX; // PMUX->SERCOM2
	
	/* configure peripheral */
	BACKLIGHT_SERCOM->SPI.CTRLA.bit.DOPO = BACKLIGHT_MOSI_PAD;            // MOSI on pad 0 (pin PA12)
	BACKLIGHT_SERCOM->SPI.CTRLA.bit.CPHA = 0x00;  
	BACKLIGHT_SERCOM->SPI.CTRLA.bit.CPOL = 0x00;                          // transfer mode 0
	BACKLIGHT_SERCOM->SPI.CTRLA.bit.DORD = 0x00;                          // MSB first
	BACKLIGHT_SERCOM->SPI.CTRLA.bit.MODE = 0x03;                          // SPI master operation
	BACKLIGHT_SERCOM->SPI.CTRLB.bit.CHSIZE = 0x00;                        // 8 bit character size
	BACKLIGHT_SERCOM->SPI.BAUD.reg = 9;                                   // SPI frequency 2.4 MHz (BAUD = gclk/(2*f_baud) - 1)
	
	/* enable peripheral */
	SERCOM2->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
}

/* Initialize nxdisplay SPI on sercom 5 */
void nxdisp_SPI_init() {
	/* enable peripheral clock */
	PM->APBCMASK.bit.SERCOM5_ = 1;
	
	GCLK->CLKCTRL.bit.ID = GCLK_CLKCTRL_ID_SERCOM5_CORE_Val;
	
	GCLK->CLKCTRL.bit.GEN = GCLK_CLKCTRL_GEN_GCLK0_Val;          // source is generic clock generator 0
	GCLK->CLKCTRL.bit.CLKEN = 1;                                 // enable generic clock

	/* select GPIO pins alternative function */
	PORT->Group[NXD_MOSI_PORT].PINCFG[NXD_MOSI_PIN].bit.PMUXEN = 1;				// enable alternative function for pin
	PORT->Group[NXD_MOSI_PORT].PMUX[NXD_MOSI_PIN/2].bit.PMUXE = NXD_MOSI_PMUX;	// PMUX->SERCOM5
	
	PORT->Group[NXD_SCK_PORT].PINCFG[NXD_SCK_PIN].bit.PMUXEN = 1;				// enable alternative function for pin
	PORT->Group[NXD_SCK_PORT].PMUX[NXD_SCK_PIN/2].bit.PMUXO = NXD_SCK_PMUX;		// PMUX->SERCOM5

	SERCOM5->SPI.CTRLA.bit.DOPO = 0x0;		// DO = Pad0, SCK = Pad1
	SERCOM5->SPI.CTRLA.bit.CPOL = 0x00;		// Falling edge active (since clock is inverted)
	SERCOM5->SPI.CTRLA.bit.DORD = 0x00;		// MSB first
	SERCOM5->SPI.CTRLA.bit.MODE = 0x03;		// SPI master operation
	SERCOM5->SPI.CTRLB.bit.CHSIZE = 0x00;	// 8 bit character size
	SERCOM5->SPI.BAUD.reg = 39;				// SPI frequency 600 kHz 48M/(2*600k) - 1
	
	// Enable the peripheral
	SERCOM5->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOM5->SPI.SYNCBUSY.bit.ENABLE); // Wait for the operation to complete
}

