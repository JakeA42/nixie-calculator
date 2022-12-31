/*
 * NXDisplay.c
 *
 * Created: 2022-12-30 11:16:51 PM
 *  Author: Jake Alt
 */ 

#include "NXDisplay.h"
#include "gpio.h"
#include "pins.h"

#include <string.h>

static NXConfig *config;

static uint8_t dispdata[NX_SR_BYTES];

void NXDisplay_loadData(uint16_t buf[NX_NUM_TUBES]) {
	//  Put array of 9 digits (12 bits each) into the byte aligned display data buffer
	for (int i = 0, j = 0; i < NX_NUM_TUBES - 1; i += 2, j +=3) {
		dispdata[j] = buf[i] & 0xff;
		dispdata[j+1] = ((buf[i] >> 8) & 0xf) | ((buf[i+1] & 0xf) << 4);
		dispdata[j+2] = (buf[i+1] >> 4) & 0xff;
	}
	dispdata[NX_SR_BYTES - 1] = buf[NX_SIGNTUBE_IDX] & 0xff;
}

void NXDisplay_updateDisp() {
	// TODO: Brightness setting
	
	#ifndef NX_USE_SERCOM
	for (int i = NX_SR_BITS; i >= 0; i--) {
		gpio_set_pin(NXD_MOSI_PORT, NXD_MOSI_PIN, ((dispdata[i / 8] >> (i % 8)) & 1));
		asm("nop\nnop\nnop\nnop\nnop");				// delay at least 100 ns
		gpio_set_pin(NXD_SCK_PORT, NXD_SCK_PIN, 0);
		asm("nop\nnop\nnop\nnop\nnop");				// delay at least 100 ns
		gpio_set_pin(NXD_SCK_PORT, NXD_SCK_PIN, 1);
	}
	asm("nop\nnop\nnop\nnop\nnop");				// delay at least 100 ns

	#else
	for (int i = 12; i >= 0; i--) {
		// Wait for data register to be ready
		while (!SERCOM5->SPI.INTFLAG.bit.DRE && !SERCOM5->SPI.INTFLAG.bit.TXC);
		SERCOM5->SPI.DATA.bit.DATA = dispdata[i];
	}
	// Wait for data register to be ready
	while (!SERCOM5->SPI.INTFLAG.bit.DRE && !SERCOM5->SPI.INTFLAG.bit.TXC);
	#endif
	gpio_set_pin(NXD_STR_PORT, NXD_STR_PIN, 0); // latch data (active low)
	asm("nop\nnop\nnop\nnop\nnop");				// delay at least 100 ns
	gpio_set_pin(NXD_STR_PORT, NXD_STR_PIN, 1);
}

void NXDisplay_dispStr(const char *text) {
	int firstChar = 0; // encountered the first character yet? (for allowing numbers with space)
	uint16_t buf[NX_NUM_TUBES];
	memset(buf, 0, sizeof(buf));
	for (int i = 0, dig = NX_NUM_TUBES - 2; dig >= 0 && text[i]; i++) {
		if (text[i] >= '0' && text[i] <= '9') {
			buf[dig] |= NUM_TO_NX(text[i] - '0');
			dig--;
		}
		else {
			switch (text[i]) {
				case '-':
				if(!firstChar) {
					buf[NX_SIGNTUBE_IDX] = NX_MINUS;
				}
				break;
				case '[':
				buf[dig] |= NX_DPL;
				break;
				case ']':
				buf[dig] |= NX_DPR;
				break;
				case '.':
				//config->dp_template = NX_DPR;
				//config->dp_space = true;
				buf[dig] |= config->dp_template; // TODO: recalc position if invalid
				if (config->dp_space)
					dig--;
			}
		}
		firstChar = (text[i] != ' ') || firstChar;
	}
	NXDisplay_loadData(buf);
	NXDisplay_updateDisp();
}
